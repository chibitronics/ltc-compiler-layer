#include <string.h>

#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "usbphy.h"
#include "usbmac.h"
#include "memio.h"

static struct USBPHY *current_usb_phy;
static void (*resumeThreadIPtr)(thread_reference_t *trp, msg_t *msg);

int usbPhyInitialized(struct USBPHY *phy) {
  if (!phy)
    return 0;

  return phy->initialized;
}

static void queue_thread(struct USBPHY *phy) {

  phy->read_queue_head++;
  phy->read_queue_head &= PHY_READ_QUEUE_MASK;
  if (phy->thread)
    resumeThreadIPtr(&phy->thread, 0);
}

static void usb_capture(struct USBPHY *phy) {

  uint8_t *samples;
  int ret;
  static uint8_t nak_pkt[] = {USB_PID_NAK};
  static uint8_t ack_pkt[] = {USB_PID_ACK};
  int queued_size = phy->queued_size;

  samples = (uint8_t *)phy->read_queue[phy->read_queue_head];

  /* If there is no data, or if there is an error, return */
  ret = usbPhyReadI(phy, samples);
  if (ret <= 0)
    return;

  /* Save the byte counter for later inspection */
  samples[11] = ret;

  switch (samples[0]) {

  case USB_PID_IN:

    /* If they requested data and we have none, send NAK */
    if (!queued_size) {
      usbPhyWriteI(phy, nak_pkt, sizeof(nak_pkt));
      break;
    }
    usbPhyWriteI(phy, phy->queued_data, queued_size);
    queue_thread(phy);
    break;

  case USB_PID_SETUP:
    queue_thread(phy);
    break;

  case USB_PID_OUT:
    queue_thread(phy);
    break;

  case USB_PID_ACK:
    /* Allow the next byte to be sent */
    phy->queued_size = 0;
    queue_thread(phy);
    usbMacTransferSuccess(phy->mac);
    break;

  case USB_PID_DATA0:
  case USB_PID_DATA1:
    usbPhyWriteI(phy, ack_pkt, sizeof(ack_pkt));
    queue_thread(phy);
    break;
  }

  return;
}

/*
 * Note that this interrupt plays fast-and-loose with ChibiOS conventions.
 * It does not call port_lock_from_isr() on entry, nor does it call
 * port_unlock_from_isr() on exit.  As far as ChibiOS is concerned, this
 * function does not exist.
 *
 * This is because standard ChibiOS IRQ housekeeping adds too much overhead
 * to this process, and we need to respond as quickly as possible.
 */

int usbPhyWritePrepare(struct USBPHY *phy, const void *buffer, int size) {

  phy->queued_data = buffer;
  phy->queued_size = size;
  return 0;
}

static void usbPhyWorker(struct USBPHY *phy) {
  while (phy->read_queue_tail != phy->read_queue_head) {
    uint8_t *in_ptr = phy->read_queue[phy->read_queue_tail];
    int count = in_ptr[11];

    usbMacProcess(phy->mac, in_ptr, count);

    // Advance to the next packet
    phy->read_queue_tail++;
    phy->read_queue_tail &= PHY_READ_QUEUE_MASK;
  }
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

static void usb_phy_fast_isr(void) {

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
