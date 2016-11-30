#include <string.h>

#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "usbphy.h"
#include "usbmac.h"
#include "memio.h"

static void (*resumeThreadIPtr)(thread_reference_t *trp, msg_t *msg);
static struct USBPHY *current_usb_phy;

int usbPhyInitialized(struct USBPHY *phy) {
  if (!phy)
    return 0;

  return phy->initialized;
}

__attribute__((section(".ramtext")))
static void queue_thread(struct USBPHY *phy) {

  phy->read_queue_head++;
  phy->read_queue_head &= PHY_READ_QUEUE_MASK;
  if (phy->thread)
    resumeThreadIPtr(&phy->thread, 0);
}

__attribute__((section(".ramtext")))
static void usb_capture(struct USBPHY *phy) {

  uint8_t *samples;
  int ret;

  samples = (uint8_t *)phy->read_queue[phy->read_queue_head];

  ret = usbPhyReadI(phy, samples);
  if (ret <= 0)
    goto out;

  /* Save the byte counter for later inspection */
  samples[11] = ret;

  if (samples[0] == USB_PID_IN) {

    /* Make sure we have queued data, and that it's for this particular EP */
    if ((!phy->queued_size)
    || (((((const uint16_t *)(samples+1))[0] >> 7) & 0xf) != phy->queued_epnum))
    {
      uint8_t pkt[] = {USB_PID_NAK};
      usbPhyWriteI(phy, pkt, sizeof(pkt));
      goto out;
    }

    queue_thread(phy);
    usbPhyWriteI(phy, phy->queued_data, phy->queued_size);
    goto out;
  }
  else if (samples[0] == USB_PID_SETUP) {
    queue_thread(phy);
    goto out;
  }
  else if (samples[0] == USB_PID_OUT) {
    queue_thread(phy);
    goto out;
  }
  else if (samples[0] == USB_PID_ACK) {
    /* Allow the next byte to be sent */
    phy->queued_size = 0;
    usbMacTransferSuccess(phy->mac);
    queue_thread(phy);
    goto out;
  }

  else if ((samples[0] == USB_PID_DATA0) || (samples[0] == USB_PID_DATA1)) {
    queue_thread(phy);
    uint8_t pkt[] = {USB_PID_ACK};
    usbPhyWriteI(phy, pkt, sizeof(pkt));
    goto out;
  }

out:

  return;
}

int usbPhyWritePrepare(struct USBPHY *phy, int epnum,
                       const void *buffer, int size)
{
  phy->queued_data = buffer;
  phy->queued_epnum = epnum;
  phy->queued_size = size;
  return 0;
}

struct USBPHY *usbPhyTestPhy(void) {

  return NULL;
}

int usbPhyProcessNextEvent(struct USBPHY *phy) {
  if (phy->read_queue_tail != phy->read_queue_head) {
    uint8_t *in_ptr = (uint8_t *)phy->read_queue[phy->read_queue_tail];
    int count = in_ptr[11];

    // Advance to the next packet (allowing us to be reentrant)
    phy->read_queue_tail++;
    phy->read_queue_tail &= PHY_READ_QUEUE_MASK;

    // Process the current packet
    usbMacProcess(phy->mac, in_ptr, count);

    return 1;
  }

  return 0;
}

void usbPhyWorker(struct USBPHY *phy) {
  while (phy->read_queue_tail != phy->read_queue_head)
    usbPhyProcessNextEvent(phy);
  return;
}

static THD_FUNCTION(usb_worker_thread, arg) {

  struct USBPHY *phy = arg;

  setThreadName("USB poll thread");
  while (1) {
    suspendThread(&phy->thread);
    usbPhyWorker(phy);
  }

  return;
}

__attribute__((section(".ramtext")))
static void usb_phy_fast_isr(void)
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
  struct USBPHY *phy = current_usb_phy;
  usb_capture(phy);

  /* Clear all pending interrupts on this port. */
  writel(0xffffffff, PORTB_ISFR);
}

/* Called when the PHY is disconnected, to prevent ChibiOS from
 * overwriting areas of memory that haven't been initialized yet.
 */
static void usb_phy_fast_isr_disabled(void) {

  writel(0xffffffff, PORTB_ISFR);
}

void usbPhyInit(struct USBPHY *phy, struct USBMAC *mac) {

  if (phy->initialized)
    return;

  resumeThreadIPtr = getSyscallAddr(147);

  attachFastInterrupt(PORTB_IRQ, usb_phy_fast_isr_disabled);
  phy->mac = mac;
  usbMacSetPhy(mac, phy);
  phy->initialized = 1;
  usbPhyDetach(phy);

  createThread(phy->waThread, sizeof(phy->waThread),
               127, usb_worker_thread, phy);
}

void usbPhyDetach(struct USBPHY *phy) {

  hookSysTick(NULL);

  /* Set both lines to 0 (clear both D+ and D-) to simulate unplug. */
  writel(phy->usbdpMask, phy->usbdpCAddr);
  writel(phy->usbdnMask, phy->usbdnCAddr);

  /* Set both lines to output */
  writel(readl(phy->usbdpDAddr) | phy->usbdpMask, phy->usbdpDAddr);
  writel(readl(phy->usbdnDAddr) | phy->usbdnMask, phy->usbdnDAddr);

  attachFastInterrupt(PORTB_IRQ, usb_phy_fast_isr_disabled);
}

void usbPhyAttach(struct USBPHY *phy) {

  current_usb_phy = phy;

  /* Hook our GPIO IRQ */
  attachFastInterrupt(PORTB_IRQ, usb_phy_fast_isr);

  /* Clear the interrupt lines, just in case there are pending IRQs.*/
  writel(0xffffffff, PORTB_ISFR);

  /* Set both lines to input */
  writel(readl(phy->usbdpDAddr) & ~phy->usbdpMask, phy->usbdpDAddr);
  writel(readl(phy->usbdnDAddr) & ~phy->usbdnMask, phy->usbdnDAddr);
}
