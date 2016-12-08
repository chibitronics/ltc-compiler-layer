#define USB_VID 0x1209
#define USB_PID 0x9317

#define BUFFER_SIZE 8
#define NUM_BUFFERS 4
#define EP_INTERVAL_MS 10

#include "USBCore.h"

#include "usbmac.h"
#include "usbphy.h"
#include "usblink.h"
#include "PluggableUSB.h"
#include "kl02.h"
#include "memio.h"

#define PCR_IRQC_LOGIC_ZERO         0x8
#define PCR_IRQC_RISING_EDGE        0x9
#define PCR_IRQC_FALLING_EDGE       0xA
#define PCR_IRQC_EITHER_EDGE        0xB
#define PCR_IRQC_LOGIC_ONE          0xC

struct usb_link_private {
	int config_num;
};
static struct usb_link_private link_priv;

static uint32_t rx_buffer[NUM_BUFFERS][BUFFER_SIZE / sizeof(uint32_t)];
static uint8_t rx_buffer_sizes[NUM_BUFFERS];
static uint8_t rx_buffer_eps[NUM_BUFFERS];
static uint8_t rx_buffer_head;
static uint8_t rx_buffer_tail;
static thread_t *waiting_read_threads[8];

static struct USBPHY usbPhy = {
  NULL,
  0,

  /* PTB4 */
  /*.usbdpIAddr =*/ FGPIOB_PDIR,
  /*.usbdpSAddr =*/ FGPIOB_PSOR,
  /*.usbdpCAddr =*/ FGPIOB_PCOR,
  /*.usbdpDAddr =*/ FGPIOB_PDDR,
  /*.usbdpShift =*/ 2,

  /* PTB3? */
  /*.usbdnIAddr =*/ FGPIOB_PDIR,
  /*.usbdnSAddr =*/ FGPIOB_PSOR,
  /*.usbdnCAddr =*/ FGPIOB_PCOR,
  /*.usbdnDAddr =*/ FGPIOB_PDDR,
  /*.usbdnShift =*/ 1,

  /*.usbdpMask  =*/ (1 << 2),
  /*.usbdnMask  =*/ (1 << 1),
};

static struct USBMAC usbMac;

const u16 STRING_LANGUAGE[2] = {
  (3<<8) | (2+2),
  0x0409  // English
};

#ifndef USB_PRODUCT
#define USB_PRODUCT     "Love to Code"
#endif

#ifndef USB_MANUFACTURER
#define USB_MANUFACTURER "Chibitronics"
#endif

const u8 STRING_PRODUCT[] = USB_PRODUCT;

const u8 STRING_MANUFACTURER[] = USB_MANUFACTURER;

//  DEVICE DESCRIPTOR
static const DeviceDescriptor USB_DeviceDescriptor =
  D_DEVICE(0x00,0x00,0x00,8,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,ISERIAL,1);

uint8_t _initEndpoints[USB_ENDPOINTS] =
{
  0,                      // Control Endpoint

  EP_TYPE_INTERRUPT_IN,   // CDC_ENDPOINT_ACM
  EP_TYPE_INTERRUPT_IN,        // CDC_ENDPOINT_IN
  EP_TYPE_INTERRUPT_OUT,       // CDC_ENDPOINT_OUT

  // Following endpoints are automatically initialized to 0
};

static uint8_t usb_data_buffer[128];
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
  if ((usb_data_buffer_position + (uint32_t)len) >= sizeof(usb_data_buffer))
    return false;

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

static int get_class_descriptor(struct USBLink *link,
                                const void *setup_ptr,
                                const void **data) {
  (void)link;
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

static int get_device_descriptor(struct USBLink *link,
                                 const void *setup_ptr,
                                 const void **data) {

  (void)link;
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

  if ((setup->wValueH == 0) && (setup->wIndex == 0) && (setup->wLength == 2)) {
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
    if (setup->wValueL == 0)
      desc_addr = (const uint8_t *)&STRING_LANGUAGE;
    else if (setup->wValueL == IPRODUCT)
      return USB_SendStringDescriptor(STRING_PRODUCT, setup->wLength);
    else if (setup->wValueL == IMANUFACTURER)
      return USB_SendStringDescriptor(STRING_MANUFACTURER, setup->wLength);
    else if (setup->wValueL == ISERIAL) {
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

  *data = (void *)desc_addr;
  return desc_length;
}

static int get_descriptor(struct USBLink *link,
                          const void *setup_ptr,
                          const void **data) {
  USBSetup *setup = (USBSetup *)setup_ptr;

  if ((setup->bmRequestType & REQUEST_TYPE) == REQUEST_STANDARD)
    return get_device_descriptor(link, setup_ptr, data);
  else 
    return get_class_descriptor(link, setup_ptr, data);
  return 0;
}

static void * get_usb_rx_buffer(struct USBLink *link,
                                uint8_t epNum,
                                int32_t *size)
{
  (void)link;
  (void)epNum;

  if (size)
    *size = sizeof(rx_buffer[0]);
  return rx_buffer[rx_buffer_head];
}

static int send_data_finished(struct USBLink *link, uint8_t epNum, const void *data)
{
  (void)link;
  (void)epNum;
  (void)data;

  return 0;
}

//  Blocking Send of data to an endpoint
int USB_Send(u8 ep, const void* d, int len) {

  int ret;

  while ((ret = usbMacSendData(&usbMac, ep & 0x7, d, len)) < 0)
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

static int received_data(struct USBLink *link,
                         uint8_t epNum,
                         uint32_t bytes,
                         const void *data)
{
  (void)link;
  (void)data;

  rx_buffer_sizes[rx_buffer_head] = bytes;
  rx_buffer_eps[rx_buffer_head] = epNum;
  memcpy(rx_buffer[rx_buffer_head], data, bytes);
  rx_buffer_head = (rx_buffer_head + 1) & (NUM_BUFFERS - 1);

  if (waiting_read_threads[epNum])
    resumeThread(&waiting_read_threads[epNum], 0);

  /* Return 0, indicating this packet is complete. */
  return 0;
}

static void set_config_num(struct USBLink *link, int configNum)
{
	struct usb_link_private *priv = (struct usb_link_private *)link->data;
	priv->config_num = configNum;
}

static struct USBLink usbLink = {
  /*.getDescriptor     = */ get_descriptor,
  /*.getReceiveBuffer  = */ get_usb_rx_buffer,
  /*.receiveData       = */ received_data,
  /*.sendData          = */ send_data_finished,
  /*.setConfigNum      = */ set_config_num,
  /*.data              = */ &link_priv,
  /*.mac               = */ NULL,
};

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

/******  KL2x Specific Interrupt Numbers ***********************/
  DMA0_IRQn                     = 0,
  DMA1_IRQn                     = 1,
  DMA2_IRQn                     = 2,
  DMA3_IRQn                     = 3,
  Reserved0_IRQn                = 4,
  FTFA_IRQn                     = 5,
  PMC_IRQn                      = 6,
  LLWU_IRQn                     = 7,
  I2C0_IRQn                     = 8,
  I2C1_IRQn                     = 9,
  SPI0_IRQn                     = 10,
  SPI1_IRQn                     = 11,
  UART0_IRQn                    = 12,
  UART1_IRQn                    = 13,
  UART2_IRQn                    = 14,
  ADC0_IRQn                     = 15,
  CMP0_IRQn                     = 16,
  TPM0_IRQn                     = 17,
  TPM1_IRQn                     = 18,
  TPM2_IRQn                     = 19,
  RTC0_IRQn                     = 20,
  RTC1_IRQn                     = 21,
  PIT_IRQn                      = 22,
  Reserved1_IRQn                = 23,
  USB_OTG_IRQn                  = 24,
  DAC0_IRQn                     = 25,
  TSI0_IRQn                     = 26,
  MCG_IRQn                      = 27,
  LPTMR0_IRQn                   = 28,
  Reserved2_IRQn                = 29,
  PINA_IRQn                     = 30,
  PINB_IRQn                     = 31,
} IRQn_Type;

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions                 */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions                 */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions                */
#define     __IO    volatile             /*!< Defines 'read / write' permissions              */

/** \ingroup    CMSIS_core_register
    \defgroup   CMSIS_NVIC  Nested Vectored Interrupt Controller (NVIC)
    \brief      Type definitions for the NVIC Registers
  @{
 */

/** \brief  Structure type to access the Nested Vectored Interrupt Controller (NVIC).
 */
typedef struct
{
  __IO uint32_t ISER[1];                 /*!< Offset: 0x000 (R/W)  Interrupt Set Enable Register           */
       uint32_t RESERVED0[31];
  __IO uint32_t ICER[1];                 /*!< Offset: 0x080 (R/W)  Interrupt Clear Enable Register          */
       uint32_t RSERVED1[31];
  __IO uint32_t ISPR[1];                 /*!< Offset: 0x100 (R/W)  Interrupt Set Pending Register           */
       uint32_t RESERVED2[31];
  __IO uint32_t ICPR[1];                 /*!< Offset: 0x180 (R/W)  Interrupt Clear Pending Register         */
       uint32_t RESERVED3[31];
       uint32_t RESERVED4[64];
  __IO uint32_t IP[8];                   /*!< Offset: 0x300 (R/W)  Interrupt Priority Register              */
}  NVIC_Type;

/*@} end of group CMSIS_NVIC */


/** \ingroup  CMSIS_core_register
    \defgroup CMSIS_SCB     System Control Block (SCB)
    \brief      Type definitions for the System Control Block Registers
  @{
 */

/** \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
  __I  uint32_t CPUID;                   /*!< Offset: 0x000 (R/ )  CPUID Base Register                                   */
  __IO uint32_t ICSR;                    /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register                  */
#if (__VTOR_PRESENT == 1)
  __IO uint32_t VTOR;                    /*!< Offset: 0x008 (R/W)  Vector Table Offset Register                          */
#else
       uint32_t RESERVED0;
#endif
  __IO uint32_t AIRCR;                   /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register      */
  __IO uint32_t SCR;                     /*!< Offset: 0x010 (R/W)  System Control Register                               */
  __IO uint32_t CCR;                     /*!< Offset: 0x014 (R/W)  Configuration Control Register                        */
       uint32_t RESERVED1;
  __IO uint32_t SHP[2];                  /*!< Offset: 0x01C (R/W)  System Handlers Priority Registers. [0] is RESERVED   */
  __IO uint32_t SHCSR;                   /*!< Offset: 0x024 (R/W)  System Handler Control and State Register             */
} SCB_Type;

/* SCB CPUID Register Definitions */
#define SCB_CPUID_IMPLEMENTER_Pos          24                                             /*!< SCB CPUID: IMPLEMENTER Position */
#define SCB_CPUID_IMPLEMENTER_Msk          (0xFFUL << SCB_CPUID_IMPLEMENTER_Pos)          /*!< SCB CPUID: IMPLEMENTER Mask */

#define SCB_CPUID_VARIANT_Pos              20                                             /*!< SCB CPUID: VARIANT Position */
#define SCB_CPUID_VARIANT_Msk              (0xFUL << SCB_CPUID_VARIANT_Pos)               /*!< SCB CPUID: VARIANT Mask */

#define SCB_CPUID_ARCHITECTURE_Pos         16                                             /*!< SCB CPUID: ARCHITECTURE Position */
#define SCB_CPUID_ARCHITECTURE_Msk         (0xFUL << SCB_CPUID_ARCHITECTURE_Pos)          /*!< SCB CPUID: ARCHITECTURE Mask */

#define SCB_CPUID_PARTNO_Pos                4                                             /*!< SCB CPUID: PARTNO Position */
#define SCB_CPUID_PARTNO_Msk               (0xFFFUL << SCB_CPUID_PARTNO_Pos)              /*!< SCB CPUID: PARTNO Mask */

#define SCB_CPUID_REVISION_Pos              0                                             /*!< SCB CPUID: REVISION Position */
#define SCB_CPUID_REVISION_Msk             (0xFUL /*<< SCB_CPUID_REVISION_Pos*/)          /*!< SCB CPUID: REVISION Mask */

/* SCB Interrupt Control State Register Definitions */
#define SCB_ICSR_NMIPENDSET_Pos            31                                             /*!< SCB ICSR: NMIPENDSET Position */
#define SCB_ICSR_NMIPENDSET_Msk            (1UL << SCB_ICSR_NMIPENDSET_Pos)               /*!< SCB ICSR: NMIPENDSET Mask */

#define SCB_ICSR_PENDSVSET_Pos             28                                             /*!< SCB ICSR: PENDSVSET Position */
#define SCB_ICSR_PENDSVSET_Msk             (1UL << SCB_ICSR_PENDSVSET_Pos)                /*!< SCB ICSR: PENDSVSET Mask */

#define SCB_ICSR_PENDSVCLR_Pos             27                                             /*!< SCB ICSR: PENDSVCLR Position */
#define SCB_ICSR_PENDSVCLR_Msk             (1UL << SCB_ICSR_PENDSVCLR_Pos)                /*!< SCB ICSR: PENDSVCLR Mask */

#define SCB_ICSR_PENDSTSET_Pos             26                                             /*!< SCB ICSR: PENDSTSET Position */
#define SCB_ICSR_PENDSTSET_Msk             (1UL << SCB_ICSR_PENDSTSET_Pos)                /*!< SCB ICSR: PENDSTSET Mask */

#define SCB_ICSR_PENDSTCLR_Pos             25                                             /*!< SCB ICSR: PENDSTCLR Position */
#define SCB_ICSR_PENDSTCLR_Msk             (1UL << SCB_ICSR_PENDSTCLR_Pos)                /*!< SCB ICSR: PENDSTCLR Mask */

#define SCB_ICSR_ISRPREEMPT_Pos            23                                             /*!< SCB ICSR: ISRPREEMPT Position */
#define SCB_ICSR_ISRPREEMPT_Msk            (1UL << SCB_ICSR_ISRPREEMPT_Pos)               /*!< SCB ICSR: ISRPREEMPT Mask */

#define SCB_ICSR_ISRPENDING_Pos            22                                             /*!< SCB ICSR: ISRPENDING Position */
#define SCB_ICSR_ISRPENDING_Msk            (1UL << SCB_ICSR_ISRPENDING_Pos)               /*!< SCB ICSR: ISRPENDING Mask */

#define SCB_ICSR_VECTPENDING_Pos           12                                             /*!< SCB ICSR: VECTPENDING Position */
#define SCB_ICSR_VECTPENDING_Msk           (0x1FFUL << SCB_ICSR_VECTPENDING_Pos)          /*!< SCB ICSR: VECTPENDING Mask */

#define SCB_ICSR_VECTACTIVE_Pos             0                                             /*!< SCB ICSR: VECTACTIVE Position */
#define SCB_ICSR_VECTACTIVE_Msk            (0x1FFUL /*<< SCB_ICSR_VECTACTIVE_Pos*/)       /*!< SCB ICSR: VECTACTIVE Mask */
#if (__VTOR_PRESENT == 1)
/* SCB Interrupt Control State Register Definitions */
#define SCB_VTOR_TBLOFF_Pos                 8                                             /*!< SCB VTOR: TBLOFF Position */
#define SCB_VTOR_TBLOFF_Msk                (0xFFFFFFUL << SCB_VTOR_TBLOFF_Pos)            /*!< SCB VTOR: TBLOFF Mask */
#endif

/* SCB Application Interrupt and Reset Control Register Definitions */
#define SCB_AIRCR_VECTKEY_Pos              16                                             /*!< SCB AIRCR: VECTKEY Position */
#define SCB_AIRCR_VECTKEY_Msk              (0xFFFFUL << SCB_AIRCR_VECTKEY_Pos)            /*!< SCB AIRCR: VECTKEY Mask */

#define SCB_AIRCR_VECTKEYSTAT_Pos          16                                             /*!< SCB AIRCR: VECTKEYSTAT Position */
#define SCB_AIRCR_VECTKEYSTAT_Msk          (0xFFFFUL << SCB_AIRCR_VECTKEYSTAT_Pos)        /*!< SCB AIRCR: VECTKEYSTAT Mask */

#define SCB_AIRCR_ENDIANESS_Pos            15                                             /*!< SCB AIRCR: ENDIANESS Position */
#define SCB_AIRCR_ENDIANESS_Msk            (1UL << SCB_AIRCR_ENDIANESS_Pos)               /*!< SCB AIRCR: ENDIANESS Mask */

#define SCB_AIRCR_SYSRESETREQ_Pos           2                                             /*!< SCB AIRCR: SYSRESETREQ Position */
#define SCB_AIRCR_SYSRESETREQ_Msk          (1UL << SCB_AIRCR_SYSRESETREQ_Pos)             /*!< SCB AIRCR: SYSRESETREQ Mask */

#define SCB_AIRCR_VECTCLRACTIVE_Pos         1                                             /*!< SCB AIRCR: VECTCLRACTIVE Position */
#define SCB_AIRCR_VECTCLRACTIVE_Msk        (1UL << SCB_AIRCR_VECTCLRACTIVE_Pos)           /*!< SCB AIRCR: VECTCLRACTIVE Mask */

/* SCB System Control Register Definitions */
#define SCB_SCR_SEVONPEND_Pos               4                                             /*!< SCB SCR: SEVONPEND Position */
#define SCB_SCR_SEVONPEND_Msk              (1UL << SCB_SCR_SEVONPEND_Pos)                 /*!< SCB SCR: SEVONPEND Mask */

#define SCB_SCR_SLEEPDEEP_Pos               2                                             /*!< SCB SCR: SLEEPDEEP Position */
#define SCB_SCR_SLEEPDEEP_Msk              (1UL << SCB_SCR_SLEEPDEEP_Pos)                 /*!< SCB SCR: SLEEPDEEP Mask */

#define SCB_SCR_SLEEPONEXIT_Pos             1                                             /*!< SCB SCR: SLEEPONEXIT Position */
#define SCB_SCR_SLEEPONEXIT_Msk            (1UL << SCB_SCR_SLEEPONEXIT_Pos)               /*!< SCB SCR: SLEEPONEXIT Mask */

/* SCB Configuration Control Register Definitions */
#define SCB_CCR_STKALIGN_Pos                9                                             /*!< SCB CCR: STKALIGN Position */
#define SCB_CCR_STKALIGN_Msk               (1UL << SCB_CCR_STKALIGN_Pos)                  /*!< SCB CCR: STKALIGN Mask */

#define SCB_CCR_UNALIGN_TRP_Pos             3                                             /*!< SCB CCR: UNALIGN_TRP Position */
#define SCB_CCR_UNALIGN_TRP_Msk            (1UL << SCB_CCR_UNALIGN_TRP_Pos)               /*!< SCB CCR: UNALIGN_TRP Mask */

/* SCB System Handler Control and State Register Definitions */
#define SCB_SHCSR_SVCALLPENDED_Pos         15                                             /*!< SCB SHCSR: SVCALLPENDED Position */
#define SCB_SHCSR_SVCALLPENDED_Msk         (1UL << SCB_SHCSR_SVCALLPENDED_Pos)            /*!< SCB SHCSR: SVCALLPENDED Mask */

/*@} end of group CMSIS_SCB */

/** \ingroup    CMSIS_core_register
    \defgroup   CMSIS_core_base     Core Definitions
    \brief      Definitions for base addresses, unions, and structures.
  @{
 */

/* Memory mapping of Cortex-M0+ Hardware */
#define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address              */
#define NVIC_BASE           (SCS_BASE +  0x0100UL)                    /*!< NVIC Base Address                 */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address */

#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct           */
#define SysTick             ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct       */
#define NVIC                ((NVIC_Type      *)     NVIC_BASE     )   /*!< NVIC configuration struct          */

#if (__MPU_PRESENT == 1)
  #define MPU_BASE          (SCS_BASE +  0x0D90UL)                    /*!< Memory Protection Unit             */
  #define MPU               ((MPU_Type       *)     MPU_BASE      )   /*!< Memory Protection Unit             */
#endif

/*@} */

/* Interrupt Priorities are WORD accessible only under ARMv6M                   */
/* The following MACROS handle generation of the register offset and byte masks */
#define _BIT_SHIFT(IRQn)         (  ((((uint32_t)(int32_t)(IRQn))         )      &  0x03UL) * 8UL)
#define _SHP_IDX(IRQn)           ( (((((uint32_t)(int32_t)(IRQn)) & 0x0FUL)-8UL) >>    2UL)      )
#define _IP_IDX(IRQn)            (   (((uint32_t)(int32_t)(IRQn))                >>    2UL)      )

#define __NVIC_PRIO_BITS          2


/** \brief  Set Interrupt Priority

    The function sets the priority of an interrupt.

    \note The priority cannot be set for every core interrupt.

    \param [in]      IRQn  Interrupt number.
    \param [in]  priority  Priority to set.
 */
static inline void NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if((int32_t)(IRQn) < 0) {
    SCB->SHP[_SHP_IDX(IRQn)] = ((uint32_t)(SCB->SHP[_SHP_IDX(IRQn)] & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
       (((priority << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
  }
  else {
    NVIC->IP[_IP_IDX(IRQn)]  = ((uint32_t)(NVIC->IP[_IP_IDX(IRQn)]  & ~(0xFFUL << _BIT_SHIFT(IRQn))) |
       (((priority << (8 - __NVIC_PRIO_BITS)) & (uint32_t)0xFFUL) << _BIT_SHIFT(IRQn)));
  }
}

#define CORTEX_NUM_VECTORS 32

/** \brief  Get Interrupt Priority

    The function reads the priority of an interrupt. The interrupt
    number can be positive to specify an external (device specific)
    interrupt, or negative to specify an internal (core) interrupt.


    \param [in]   IRQn  Interrupt number.
    \return             Interrupt Priority. Value is aligned automatically to the implemented
                        priority bits of the microcontroller.
 */
static inline uint32_t NVIC_GetPriority(IRQn_Type IRQn)
{

  if((int32_t)(IRQn) < 0) {
    return((uint32_t)(((SCB->SHP[_SHP_IDX(IRQn)] >> _BIT_SHIFT(IRQn) ) & (uint32_t)0xFFUL) >> (8 - __NVIC_PRIO_BITS)));
  }
  else {
    return((uint32_t)(((NVIC->IP[ _IP_IDX(IRQn)] >> _BIT_SHIFT(IRQn) ) & (uint32_t)0xFFUL) >> (8 - __NVIC_PRIO_BITS)));
  }
}

/** \brief  Enable External Interrupt

    The function enables a device-specific interrupt in the NVIC interrupt controller.

    \param [in]      IRQn  External interrupt number. Value cannot be negative.
 */
static inline void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  NVIC->ISER[0] = (uint32_t)(1UL << (((uint32_t)(int32_t)IRQn) & 0x1FUL));
}

static uint8_t usb_started = 0;
int usbStart(void) {

  if (usb_started)
    return 0;

  usbMacInit(&usbMac, &usbLink);
  usbPhyInit(&usbPhy, &usbMac);

  /* Enable the IRQ and mux as GPIO with slow slew rate */
  writel(0x000B0104, PORTB_PCR1);
  writel(0x000B0104, PORTB_PCR2);

  /* Enable the PORTB IRQ, with the highest possible priority.*/
  {
    int i;
    NVIC_SetPriority(SVCall_IRQn, 1);
    NVIC_SetPriority(PendSV_IRQn, 2);
    NVIC_SetPriority(SysTick_IRQn, 2);
    for (i = 0; i < 32; i++)
      NVIC_SetPriority((IRQn_Type)i, 3);
    NVIC_SetPriority(PINB_IRQn, 0);
    NVIC_EnableIRQ(PINB_IRQn);
  }

  pinMode(LED_BUILTIN, OUTPUT);

  delay(200);
  usbPhyAttach(&usbPhy);

  usb_started = 1;
  return 0;
}
