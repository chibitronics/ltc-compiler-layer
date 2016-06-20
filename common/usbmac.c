#include <string.h>
#include "Arduino.h"

#include "usbphy.h"
#include "usbmac.h"
#include "usblink.h"

extern uint16_t crc16(const uint8_t *data, uint32_t count,
                      uint16_t init, uint32_t poly);

static void usb_mac_process_data(struct USBMAC *mac, int epnum) {

  uint16_t crc;

  /* Don't allow us to re-prepare data */
  if (mac->packet_queued[epnum])
    return;

  /* If there's no data to send, then don't send any */
  if (!mac->data_out[epnum])
    return;

  /* If we've sent all of our data, then there's nothing else to send */
  if ((mac->data_out_left[epnum] == 0) && (mac->data_out_max[epnum] == 0)) {
    mac->data_out[epnum] = NULL;
    return;
  }

  /* Pick the correct PID, DATA0 or DATA1 */
  if (mac->data_buffer++ & 1)
    mac->packet[epnum].pid = USB_PID_DATA1;
  else
    mac->packet[epnum].pid = USB_PID_DATA0;

  /* If there's no data, prepare a special NULL packet */
  if ((mac->data_out_left[epnum] <= 0) || (mac->data_out_max[epnum] <= 0)) {
    mac->data_out_left[epnum] = 0;
    mac->data_out_max[epnum] = 0;
    mac->data_out[epnum] = NULL;
    mac->packet[epnum].data[0] = 0;
    mac->packet[epnum].data[1] = 0;
    mac->packet[epnum].size = 2; // Only two bytes of CRC
    mac->packet_queued[epnum] = 1;
    return;
  }

  /* Keep the packet size to 8 bytes max */
  if (mac->data_out_left[epnum] > 8)
    mac->packet[epnum].size = 8;
  else
    mac->packet[epnum].size = mac->data_out_left[epnum];

  /* Limit the amount of data transferred to data_out_max */
  if (mac->packet[epnum].size > mac->data_out_max[epnum])
    mac->packet[epnum].size = mac->data_out_max[epnum];

  /* Copy over data bytes */
  memcpy(mac->packet[epnum].data, mac->data_out[epnum], mac->packet[epnum].size);

  /* Calculate and copy the crc16 */
  crc = ~crc16(mac->packet[epnum].data, mac->packet[epnum].size, 0xffff, 0xa001);
  mac->packet[epnum].data[mac->packet[epnum].size++] = crc;
  mac->packet[epnum].data[mac->packet[epnum].size++] = crc >> 8;

  /* Prepare the packet, including the PID at the end */
  mac->packet_queued[epnum] = 1;
}

static int usb_mac_send_data(struct USBMAC *mac,
                             uint8_t epnum,
                             const void *data,
                             int count,
                             int max) {

  mac->data_out[epnum] = data;
  mac->data_out_left[epnum] = count;
  mac->data_out_max[epnum] = max;
  usb_mac_process_data(mac, epnum);

  return 0;
}

void usbMacTransferSuccess(struct USBMAC *mac, int epnum) {

  mac->packet_queued[epnum] = 0;

  /* Reduce the amount of data left.
   * If the packet is divisible by 8, this will cause one more call
   * to this function with mac->data_out_left == 0.  This will send
   * a NULL packet, which indicates end-of-transfer.
   */
  if (mac->data_out[epnum]) {
    mac->data_out_left[epnum] -= 8;
    mac->data_out_max[epnum] -= 8;
    mac->data_out[epnum] += 8;
  }
  if ((mac->data_out_left[epnum] < 0) || (mac->data_out_max[epnum] < 0)) {
    mac->data_out_left[epnum] = 0;
    mac->data_out_max[epnum] = 0;
    mac->data_out[epnum] = NULL;

    /*
    if (mac->data_out_queue_tail != mac->data_out_queue_head) {
      usb_mac_send_data(mac,
                        mac->data_out_queues[mac->data_out_queue_tail],
                        mac->data_out_queue_sizes[mac->data_out_queue_tail],
                        mac->data_out_queue_sizes[mac->data_out_queue_tail]);
      mac->data_out_queue_tail++;
      mac->data_out_queue_tail &= (MAX_OUT_QUEUES-1);
    }
    */
  }

//  usb_mac_process_data(mac);
}

int usbSendData(struct USBMAC *mac, int epnum, const void *data, int count) {

  (void)epnum;
  int ret;

  ret = usb_mac_send_data(mac, epnum, data, count, count);
  if (ret)
    return ret;

  suspendThread(&mac->threads[epnum]);

  return 0;
}

#if 0
int usbQueueData(struct USBMAC *mac, int epnum, const void *data, int size) {

  /*
  mac->data_out_queue_sizes[mac->data_out_queue_head] = size;
  mac->data_out_queues[mac->data_out_queue_head] = data;
  mac->data_out_queue_head++;
  mac->data_out_queue_head &= (MAX_OUT_QUEUES-1);
  */

  return 0;
}

int usbSendQueue(struct USBMAC *mac, int epnum) {

  /*
  if (mac->data_out_queue_tail == mac->data_out_queue_head)
    return -1;

  usb_mac_send_data(mac,
                    mac->data_out_queues[mac->data_out_queue_tail],
                    mac->data_out_queue_sizes[mac->data_out_queue_tail],
                    mac->data_out_queue_sizes[mac->data_out_queue_tail]);
  mac->data_out_queue_tail++;
  mac->data_out_queue_tail &= (MAX_OUT_QUEUES-1);
  usb_mac_process_data(mac);
  */

  /* Wait for the queued data to drain out */
  /*
  while (mac->data_out_queue_tail != mac->data_out_queue_head)
    suspendThread(&mac->thread);
  */

  return 0;
}
#endif

__attribute__((section(".ramtext")))
int macGetEndpointData(struct USBMAC *mac, int epnum,
                       struct usb_packet **queued_data) {

  if (!mac->packet_queued[epnum])
    return 0;

  *queued_data = &mac->packet[epnum];
  return 1;
}

static int usb_mac_process_setup_read(struct USBMAC *mac,
                                      const struct usb_mac_setup_packet *setup)
{
  void *response = NULL;
  uint32_t len = 0;
  struct USBLink *link = mac->link;

  len = link->getDescriptor(link, setup, &response);
  usb_mac_send_data(mac, mac->tok_epnum, response, len, setup->wLength);

  return 0;
}

static int usb_mac_process_setup_write(struct USBMAC *mac,
                                       const struct usb_mac_setup_packet *setup)
{
  static const char *response = "";
  int len = 0;
  int max = 1;
  struct USBLink *link = mac->link;

  switch (setup->bmRequestType) {
  case 0x00:  /* Device-to-host, standard, write to host */
    switch (setup->bRequest) {
    case 5: /* SET_ADDRESS */
      mac->address = setup->wValue;
      break;

    case 9: /* SET_CONFIGURATION */
      link->config_num = setup->wValue;
      break;
    }
    break;

  case 0x01: /* Device-to-host, standard, write to host */
    break;

  case 0x02: /* Device-to-host, standard, write to host */
    break;

  case 0x03: /* Device-to-host, standard, write to host */
    break;

  case 0x20: /* Device-to-host, class, write to host */
    break;

  case 0x21: /* Device-to-host, class, write to host */
    switch (setup->bRequest) {
    case 0x0a: /* Thingy? */
      /* This happens in Set Idle */
      break;
    }
    break;

  case 0x22: /* Device-to-host, class, write to host */
    break;

  case 0x23: /* Device-to-host, class, write to host */
    break;

  case 0x40: /* Device-to-host, vendor, write to host */
    break;

  case 0x41: /* Device-to-host, vendor, write to host */
    break;

  case 0x42: /* Device-to-host, vendor, write to host */
    break;

  case 0x43: /* Device-to-host, vendor, write to host */
    break;
  }

  /* We must always send a response packet.  If there's ever a time when
   * we shouldn't send a packet, simply "return" rather than "break" above.
   */
  usb_mac_send_data(mac, mac->tok_epnum, response, len, max);

  return 0;
}

static int usb_mac_process_setup(struct USBMAC *mac, const uint32_t packet[3]) {

  const struct usb_mac_setup_packet *setup;

  setup = (const struct usb_mac_setup_packet *)packet;

  if (setup->bmRequestType & 0x80)
    /* Device-to-Host */
    return usb_mac_process_setup_read(mac, setup);

  else
    return usb_mac_process_setup_write(mac, setup);
}

static void usb_mac_parse_token(struct USBMAC *mac,
                                const uint8_t packet[2]) {

  mac->tok_addr = packet[0] & 0x7f;
  mac->tok_epnum = ((packet[0] >> 7) & 1) | ((packet[1] << 1) & 0xe);
}

static void usb_mac_parse_data(struct USBMAC *mac,
                               const uint8_t packet[10],
                               uint32_t count) {
  (void)count;
  uint32_t packet_aligned[3];

  memcpy(packet_aligned, packet, count);

  switch (mac->packet_type) {

  case packet_type_setup:
    usb_mac_process_setup(mac, packet_aligned);
    break;

  case packet_type_in:

    break;

  case packet_type_out:

    break;

  case packet_type_none:
    break;

  default:
    break;
  }
}

int usbMacProcess(struct USBMAC *mac,
                  const uint8_t packet[11],
                  uint32_t count) {

  switch(packet[0]) {
  case USB_PID_SETUP:
    mac->packet_type = packet_type_setup;
    usb_mac_parse_token(mac, packet + 1);
    break;

  case USB_PID_DATA0:
    mac->data_buffer = 1;
    usb_mac_parse_data(mac, packet + 1, count - 1);
    mac->packet_type = packet_type_none;
    break;

  case USB_PID_DATA1:
    mac->data_buffer = 0;
    usb_mac_parse_data(mac, packet + 1, count - 1);
    mac->packet_type = packet_type_none;
    break;

  case USB_PID_ACK:
    if (mac->threads[mac->tok_epnum] &&
        (!mac->data_out[mac->tok_epnum] ||
          ((mac->data_out_left[mac->tok_epnum] == 0) && (mac->data_out_max[mac->tok_epnum] == 0))) ) {
      mac->data_out_left[mac->tok_epnum] = 0;
      mac->data_out_max[mac->tok_epnum] = 0;
      mac->data_out[mac->tok_epnum] = NULL;
      resumeThread(&mac->threads[mac->tok_epnum], 0);
    }
    break;

  case USB_PID_IN:
    mac->packet_type = packet_type_in;
    usb_mac_parse_token(mac, packet + 1);
    break;

  case USB_PID_OUT:
    mac->packet_type = packet_type_out;
    usb_mac_parse_token(mac, packet + 1);
    break;

  default:
    break;
  }

  /* Make sure we don't have any hanging threads */
  /*
  int i;
  for (i = 0; i < MAX_EPS; i++) {
    if (mac->threads[i] && !mac->packet_queued[i]) {
      resumeThread(&mac->threads[i], -1);
    }
  }
  */

  /* Pre-process any data that's remaining, so it can be sent out
   * in case an IN token comes.
   */
  usb_mac_process_data(mac, mac->tok_epnum);

  return 0;
}

void usbMacInit(struct USBMAC *mac, struct USBLink *link) {

  mac->link = link;
  memset(mac->data_out, 0, sizeof(mac->data_out));
  memset(mac->data_out_left, 0, sizeof(mac->data_out_left));
  memset(mac->data_out_max, 0, sizeof(mac->data_out_max));
  mutexInit(&mac->access_mutex);
}

void usbMacSetPhy(struct USBMAC *mac, struct USBPHY *phy) {

  mac->phy = phy;
}

void usbMacSetLink(struct USBMAC *mac, struct USBLink *link) {

  mac->link = link;
}

struct USBPHY *usbMacPhy(struct USBMAC *mac) {

  return mac->phy;
}

struct USBMAC *usbMacDefault(void) {

  return NULL;
}

void usbSendControl(void) {
  return;
}

void usbReceiveControl(void) {
  return;
}
