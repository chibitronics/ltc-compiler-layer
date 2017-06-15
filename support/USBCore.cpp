#define USB_VID 0x1209
#define USB_PID 0xC1B1

#define BUFFER_SIZE 16
#define NUM_BUFFERS 4
#define EP_INTERVAL_MS 10

#include "USBCore.h"
#include "ChibiOS.h"

#include "grainuum.h"
#include "PluggableUSB.h"
#include "kl02.h"
#include "memio.h"

static thread_reference_t phyThread;
static THD_WORKING_AREA(waPhyThread, 192);

static GRAINUUM_BUFFER(phy_buffer, 8);
static void (*resumeThreadIPtr)(thread_reference_t *trp, msg_t *msg);
static void (*lockFromISR)();
static void (*unlockFromISR)();

static uint32_t rx_buffer[NUM_BUFFERS][BUFFER_SIZE / sizeof(uint32_t)];
static uint8_t rx_buffer_sizes[NUM_BUFFERS];
static uint8_t rx_buffer_eps[NUM_BUFFERS];
static uint8_t rx_buffer_head;
static uint8_t rx_buffer_tail;
static thread_t *waiting_read_threads[8];

static struct GrainuumUSB usbPhy = {
  NULL,
  0,

  /* PTB1 */
  /*.usbdpIAddr =*/ FGPIOB_PDIR,
  /*.usbdpSAddr =*/ FGPIOB_PSOR,
  /*.usbdpCAddr =*/ FGPIOB_PCOR,
  /*.usbdpDAddr =*/ FGPIOB_PDDR,
  /*.usbdpShift =*/ 1,

  /* PTB2 */
  /*.usbdnIAddr =*/ FGPIOB_PDIR,
  /*.usbdnSAddr =*/ FGPIOB_PSOR,
  /*.usbdnCAddr =*/ FGPIOB_PCOR,
  /*.usbdnDAddr =*/ FGPIOB_PDDR,
  /*.usbdnShift =*/ 2,

  /*.usbdpMask  =*/ (1 << 1),
  /*.usbdnMask  =*/ (1 << 2),
};

static const u16 STRING_LANGUAGE[2] = {
  (3<<8) | (2+2),
  0x0409  // English
};

#ifndef USB_PRODUCT
#define USB_PRODUCT     "Love to Code"
#endif

#ifndef USB_MANUFACTURER
#define USB_MANUFACTURER "Chibitronics"
#endif

static const u8 STRING_PRODUCT[] = USB_PRODUCT;

static const u8 STRING_MANUFACTURER[] = USB_MANUFACTURER;

//  DEVICE DESCRIPTOR
static const DeviceDescriptor USB_DeviceDescriptor =
  D_DEVICE(0x00,0x00,0x00,8,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,0,1);

uint8_t _initEndpoints[USB_ENDPOINTS] =
{
  0,                      // Control Endpoint
  // Following endpoints are automatically initialized to 0
};

static uint8_t *usb_data_buffer;
static uint32_t usb_data_buffer_size;
static uint16_t usb_data_buffer_position;
static const void *usb_data_buffer_ptr;
static uint32_t usb_data_buffer_ptr_len = 0;

/* Arduino routines call this function to send packets over USB.
 * Our system is a pull-rather-than-push, so use this to fill up a
 * buffer instead of sending data immediately.
 * This is probably wasteful of memory.
 */
int USB_SendControl(u8 flags, const void* d, int len) {

  (void)flags;
  if ((usb_data_buffer_position + (uint32_t)len) >= usb_data_buffer_size) {
    usb_data_buffer_size = usb_data_buffer_position + len;
    usb_data_buffer = (uint8_t *)realloc(usb_data_buffer, usb_data_buffer_size);
  }

  memcpy(usb_data_buffer + usb_data_buffer_position, d, len);
  usb_data_buffer_position += len;

  return true;
}

int USB_SendEntireControl(const void *d, int len) {
  usb_data_buffer_ptr = d;
  usb_data_buffer_ptr_len = len;
  return len;
}

static int USB_SendConfiguration(int maxlen) {
  uint8_t interfaces = 0;
  
  /* Fill in the config descriptor payload first, then fill in the header.*/
  usb_data_buffer_position = sizeof(ConfigDescriptor);

  /* Call PluggableUSB to "send" all the interfaces to the host.
   * In reality, we capture everything it sends so we can send it all
   * as one packet just below.
   */
  PluggableUSB().getInterface(&interfaces);

  /* Construct the config descriptor using the information we just got
   * about buffer sizes.
   */
  ConfigDescriptor config = D_CONFIG(usb_data_buffer_position, interfaces);

  /* Stick the config descriptor at the head of the buffer we're sending.*/
  memcpy(usb_data_buffer, &config, sizeof(config));

  /* Now that usb_data_buffer is populated, the callee will pass it to
   * the USB subsystem to send.
   */
  return (usb_data_buffer_position > maxlen) ? maxlen : usb_data_buffer_position;
}

static int USB_SendStringDescriptor(const uint8_t *string_P,
                                    uint32_t string_len) {

  uint32_t l = strlen((char *)string_P);
  if (l < string_len)
    string_len = l;

  usb_data_buffer[usb_data_buffer_position++] = 2 + string_len * 2;
  usb_data_buffer[usb_data_buffer_position++] = 3;

  for(u8 i = 0; i < string_len && string_P[i]; i++) {
    usb_data_buffer[usb_data_buffer_position++] = string_P[i];
    usb_data_buffer[usb_data_buffer_position++] = 0; /* UTF-16 high byte */
  }
  return usb_data_buffer_position;
}

static int get_class_descriptor(struct GrainuumUSB *usb,
                                const void *setup_ptr,
                                const void **data) {
  (void)usb;
  USBSetup *setup = (USBSetup *)setup_ptr;
  usb_data_buffer_position = 0;
  *data = (void *)usb_data_buffer;
  usb_data_buffer_ptr = NULL;
  usb_data_buffer_ptr_len = 0;

  PluggableUSB().setup(*setup);

  if (usb_data_buffer_ptr) {
    *data = usb_data_buffer_ptr;
    return usb_data_buffer_ptr_len;
  }
  return usb_data_buffer_position;
}

static int get_device_descriptor(struct GrainuumUSB *usb,
                                 const void *setup_ptr,
                                 const void **data) {

  (void)usb;
  USBSetup *setup = (USBSetup *)setup_ptr;
  uint8_t t = setup->wValueH;
  int desc_length = 0;
  int ret = 0;
  const uint8_t *desc_addr = 0;

  usb_data_buffer_position = 0;
  *data = (void *)usb_data_buffer;
  usb_data_buffer_ptr = 0;

  if ((setup->bmRequestType & REQUEST_DIRECTION) == REQUEST_HOSTTODEVICE)
    return 0;

  if (USB_CONFIGURATION_DESCRIPTOR_TYPE == t)
      return USB_SendConfiguration(setup->wLength);

  /* See if the PluggableUSB module will handle this request */
  ret = PluggableUSB().getDescriptor(*setup);
  if (ret) {
    if (ret > 0) {
      if (usb_data_buffer_ptr) {
        *data = usb_data_buffer_ptr;
        return usb_data_buffer_ptr_len;
      }
      return usb_data_buffer_position;
    }
    return 0;
  }

  if ((setup->wValueL == 0) && (setup->wIndex == 0) && (setup->wLength == 2)) {
    static const uint8_t okay[] = {0, 0};
    *data = okay;
    return sizeof(okay);
  }
  switch (t) {
  case USB_DEVICE_DESCRIPTOR_TYPE:
    desc_addr = (const uint8_t*)&USB_DeviceDescriptor;
    if (*desc_addr > setup->wLength)
      desc_length = setup->wLength;
    break;

  case USB_STRING_DESCRIPTOR_TYPE:
    if (setup->wValueH == 0)
      desc_addr = (const uint8_t *)&STRING_LANGUAGE;
    else if (setup->wValueH == IPRODUCT)
      return USB_SendStringDescriptor(STRING_PRODUCT, setup->wLength);
    else if (setup->wValueH == IMANUFACTURER)
      return USB_SendStringDescriptor(STRING_MANUFACTURER, setup->wLength);
    else if (setup->wValueH == ISERIAL) {
      char name[ISERIAL_MAX_LEN] = {};
      PluggableUSB().getShortName(name);
      return USB_SendStringDescriptor((uint8_t*)name, setup->wLength);
    }
    else
      return false;

    if (*desc_addr > setup->wLength)
      desc_length = setup->wLength;
    break;

  default:
    //printf("Device ERROR");
    break;
  }

  /* If the calls above haven't returned, or set a descriptor address,
   * return 0 bytes, which will send an empty descriptor. */
  if (desc_addr == 0)
    return 0;

  /* Descriptors store their length in the first byte.  If we don't have
   * a descriptor length set yet, use this value.
   */
  if (desc_length == 0)
    desc_length = *desc_addr;

  *data = (const void *)desc_addr;
  return desc_length;
}

static int get_descriptor(struct GrainuumUSB *usb,
                          const void *setup_ptr,
                          const void **data) {
  const USBSetup *setup = (const USBSetup *)setup_ptr;

  if ((setup->bmRequestType & REQUEST_TYPE) == REQUEST_STANDARD)
    return get_device_descriptor(usb, setup_ptr, data);
  else 
    return get_class_descriptor(usb, setup_ptr, data);
  return 0;
}

static void * get_usb_rx_buffer(struct GrainuumUSB *usb,
                                uint8_t epNum,
                                int32_t *size)
{
  (void)usb;
  (void)epNum;

  if (size)
    *size = sizeof(rx_buffer[0]);
  return rx_buffer[rx_buffer_head];
}

static int send_data_finished(struct GrainuumUSB *usb,
                              int result)
{
  (void)usb;
  (void)result;

  return 0;
}

//  Blocking Send of data to an endpoint
static void *data_buffer;
static uint32_t data_buffer_size;
int USB_Send(u8 ep, const void* d, int len) {

  int ret;

  if ((uint32_t)len > data_buffer_size) {
    data_buffer = realloc(data_buffer, data_buffer_size);
  }
  memcpy(data_buffer, d, len);
  while ((ret = grainuumSendData(&usbPhy, ep & 0x7, data_buffer, len)) < 0)
    threadSleep(ST2MS(1));

  return ret;
}

int USB_RecvControl(void* d, int len) {
    return USB_Recv(0, d, len);
}

uint8_t USB_SendSpace(uint8_t ep);

uint8_t USB_Available(uint8_t ep) {

  if (rx_buffer_tail == rx_buffer_head)
    return 0;

  if (ep != rx_buffer_eps[rx_buffer_tail])
    return 0;

  return 1;
}

// Non-blocking endpoint reception.
int USB_Recv(uint8_t ep, void* data, int len) {

  int bytes_to_copy;

  if (!USB_Available(ep))
    return 0;

  bytes_to_copy = rx_buffer_sizes[rx_buffer_tail];
  if (len < rx_buffer_sizes[rx_buffer_tail])
    bytes_to_copy = len;

  memcpy(data, rx_buffer[rx_buffer_tail], bytes_to_copy);
  rx_buffer_tail = (rx_buffer_tail + 1) & (NUM_BUFFERS - 1);

  return bytes_to_copy;
}

int USB_Recv(uint8_t ep) {
  uint8_t c;
  if (USB_Recv(ep,&c,1) != 1)
    return -1;
  return c;
}                           // non-blocking

int USB_RecvWait(uint8_t ep, void *data, int len)
{
  if (!USB_Available(ep))
    suspendThread(&waiting_read_threads[ep]);
  return USB_Recv(ep, data, len);
}

void USB_Flush(uint8_t ep) {
    (void)ep;
    return;
}

static int received_data(struct GrainuumUSB *usb,
                         uint8_t epNum,
                         uint32_t bytes,
                         const void *data)
{
  (void)usb;
  (void)data;

  rx_buffer_sizes[rx_buffer_head] = bytes;
  rx_buffer_eps[rx_buffer_head] = epNum;
  memcpy(rx_buffer[rx_buffer_head], data, bytes);

  // Advance the receive fifo
  rx_buffer_head = (rx_buffer_head + 1) & (NUM_BUFFERS - 1);

  if (waiting_read_threads[epNum])
    resumeThread(&waiting_read_threads[epNum], 0);

  /* Return 0, indicating this packet is complete. */
  return 0;
}

static struct GrainuumConfig usbConfig = {
  /*.getDescriptor     = */ get_descriptor,
  /*.setConfigNum      = */ NULL,
  /*.getReceiveBuffer  = */ get_usb_rx_buffer,
  /*.receiveData       = */ received_data,
  /*.sendDataStarted   = */ NULL,
  /*.sendDataFinished  = */ send_data_finished,
  /*.data              = */ NULL,
  /*.usb               = */ NULL
};

static int usb_phy_process_next_event(struct GrainuumUSB *usb) {
  if (!GRAINUUM_BUFFER_IS_EMPTY(phy_buffer)) {

    __disable_irq();
    uint8_t *in_ptr = GRAINUUM_BUFFER_TOP(phy_buffer);

    // Advance to the next packet (allowing us to be reentrant)
    GRAINUUM_BUFFER_REMOVE(phy_buffer);
    __enable_irq();

    // Process the current packet
    grainuumProcess(usb, in_ptr);

    return 1;
  }

  return 0;
}

static THD_FUNCTION(usb_worker_thread, arg) {

  struct GrainuumUSB *usb = (struct GrainuumUSB *)arg;

  setThreadName("USB poll thread");
  while (1) {
    suspendThreadTimeout(&phyThread, ST2MS(10));
    while (!GRAINUUM_BUFFER_IS_EMPTY(phy_buffer))
      usb_phy_process_next_event(usb);
  }

  return;
}

__attribute__((section(".ramtext")))
static int usb_phy_fast_isr(void)
{
  /* Note: We can't use ANY ChibiOS threads here.
   * This thread may interrupt the SysTick handler, which would cause
   * Major Problems if we called OSAL_IRQ_PROLOGUE().
   * To get around this, we simply examine the buffer every time SysTick
   * exits (via CH_CFG_SYSTEM_TICK_HOOK), and wake up the thread from
   * within the SysTick context.
   * That way, this function is free to preempt EVERYTHING without
   * interfering with the timing of the system.
   */
  grainuumCaptureI(&usbPhy, GRAINUUM_BUFFER_ENTRY(phy_buffer));

  /* Clear all pending interrupts on this port. */
  writel(0xffffffff, PORTB_ISFR);
  return 0;
}

/* Called when the PHY is disconnected, to prevent ChibiOS from
 * overwriting areas of memory that haven't been initialized yet.
 */
static int usb_phy_fast_isr_disabled(void) {

  writel(0xffffffff, PORTB_ISFR);
  return 0;
}

__attribute__((section(".ramtext")))
void grainuumReceivePacket(struct GrainuumUSB *usb)
{
  GRAINUUM_BUFFER_ADVANCE(phy_buffer);

  if (phyThread) {
    lockFromISR();
    resumeThreadIPtr(&phyThread, 0);
    unlockFromISR();
  }
}

/*
 * ==============================================================
 * ---------- Interrupt Number Definition -----------------------
 * ==============================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ****************/
  Reset_IRQn                    = -15,
  NonMaskableInt_IRQn           = -14,
  HardFault_IRQn                = -13,
  SVCall_IRQn                   = -5,
  PendSV_IRQn                   = -2,
  SysTick_IRQn                  = -1,
} IRQn_Type;

void grainuumConnectPre(struct GrainuumUSB *usb)
{
  (void)usb;
  /* Hook our GPIO IRQ */
  attachFastInterrupt(PORTB_IRQ, usb_phy_fast_isr);
}

void grainuumDisconnectPre(struct GrainuumUSB *usb)
{
  (void)usb;
//  hookSysTick(NULL);
}

void grainuumDisconnectPost(struct GrainuumUSB *usb)
{
  (void)usb;
  attachFastInterrupt(PORTB_IRQ, usb_phy_fast_isr_disabled);
}

static uint8_t usb_started = 0;
int usbStart(void) {

  if (usb_started)
    return 0;

  resumeThreadIPtr = (void (*)(thread_t**, msg_t*)) getSyscallAddr(147);
  lockFromISR = (void (*)()) getSyscallAddr(132);
  unlockFromISR = (void (*)()) getSyscallAddr(133);

  GRAINUUM_BUFFER_INIT(phy_buffer);
  attachFastInterrupt(PORTB_IRQ, usb_phy_fast_isr_disabled);

  /* First batch of stickers reported ABI 1.  The pins were swapped on that version. */
  if (getSyscallABI() == 1) {
    usbPhy.usbdpShift = 2;
    usbPhy.usbdnShift = 1;
    usbPhy.usbdpMask  = (1 << 2);
    usbPhy.usbdnMask  = (1 << 1);
  }

  grainuumDisconnect(&usbPhy);
  grainuumInit(&usbPhy, &usbConfig);

  memset(waPhyThread, 0xaa, sizeof(waPhyThread));
  createThread(waPhyThread, sizeof(waPhyThread),
               127, usb_worker_thread, &usbPhy);

  /* Enable the IRQ and mux as GPIO with slow slew rate */
  writel(0x000B0104, PORTB_PCR1);
  writel(0x000B0104, PORTB_PCR2);

  /* Enable the PORTB IRQ, with the highest possible priority.*/
  {
    int i;
    setInterruptPriority(SVCall_IRQn, 1);
    setInterruptPriority(PendSV_IRQn, 2);
    setInterruptPriority(SysTick_IRQn, 2);
    for (i = 0; i < 32; i++)
      setInterruptPriority(i, 3);
    setInterruptPriority(PORTB_IRQ, 0);
    enableInterrupt(PORTB_IRQ);
  }

  delay(500);
  grainuumConnect(&usbPhy);

  usb_started = 1;
  return 0;
}
