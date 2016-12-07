#include <string.h>
#include "Arduino.h"
#include "usbphy.h"
#include "usbmac.h"
#include "usblink.h"

struct USBMAC default_mac;

static uint16_t crc16_add(uint16_t crc, uint8_t c, uint16_t poly)
{
  uint8_t  i;

  for (i = 0; i < 8; i++) {
    if ((crc ^ c) & 1)
      crc = (crc >> 1) ^ poly;
    else
      crc >>= 1;
    c >>= 1;
  }
  return crc;
}

static uint16_t crc16(const uint8_t *data, uint32_t count,
                      uint16_t init, uint32_t poly) {

  while (count--)
    init = crc16_add(init, *data++, poly);

  return init;
}

static void usb_mac_clear_tx(struct USBMAC *mac, int result)
{
  /* If a thread is blocking, wake it up with a failure */
  if (mac->thread)
    resumeThread(&mac->thread, result);
  mac->data_out_left = 0;
  mac->data_out_max = 0;
  mac->data_out = NULL;
  mac->packet_queued = 0;
}

static void usb_mac_process_tx(struct USBMAC *mac) {

  uint16_t crc;

  /* Don't allow us to re-prepare data */
  if (mac->packet_queued) {
    return;
  }
  mac->packet_queued = 1;

  /* If there's no data to send, then don't send any */
  if (!mac->data_out) {
    mac->packet_queued = 0;
    return;
  }

  /* If we've sent all of our data, then there's nothing else to send */
  if ((mac->data_out_left < 0) || (mac->data_out_max < 0)) {
    usb_mac_clear_tx(mac, 0);
    return;
  }

  /* Pick the correct PID, DATA0 or DATA1 */
  if (mac->data_buffer & (1 << mac->tok_epnum))
    mac->packet.pid = USB_PID_DATA1;
  else
    mac->packet.pid = USB_PID_DATA0;

  /* If there's no data, prepare a special NULL packet */
  if ((mac->data_out_left == 0) || (mac->data_out_max == 0)) {

    /* The special-null thing only happens for EP0 */
    if (mac->data_out_epnum != 0) {
      usb_mac_clear_tx(mac, 0);
      return;
    }
    mac->packet.data[0] = 0;  /* CRC16 for empty packets is 0 */
    mac->packet.data[1] = 0;
    mac->packet.size = 2;
    usbPhyWritePrepare(mac->phy, mac->data_out_epnum,
                       &mac->packet, mac->packet.size + 1);
    return;
  }

  /* Keep the packet size to 8 bytes max */
  if (mac->data_out_left > 8)
    mac->packet.size = 8;
  else
    mac->packet.size = mac->data_out_left;

  /* Limit the amount of data transferred to data_out_max */
  if (mac->packet.size > mac->data_out_max)
    mac->packet.size = mac->data_out_max;

  /* Copy over data bytes */
  memcpy(mac->packet.data, mac->data_out, mac->packet.size);

  /* Calculate and copy the crc16 */
  crc = ~crc16(mac->packet.data, mac->packet.size, 0xffff, 0xa001);
  mac->packet.data[mac->packet.size++] = crc;
  mac->packet.data[mac->packet.size++] = crc >> 8;

  /* Prepare the packet, including the PID at the end */
  usbPhyWritePrepare(mac->phy, mac->data_out_epnum,
                     &mac->packet, mac->packet.size + 1);
}

/* Called when a packet is ACKed.
 * Updates the outgoing packet buffer.
 */
static void usbMacTransferSuccess(struct USBMAC *mac) {

  /* Reduce the amount of data left.
   * If the packet is divisible by 8, this will cause one more call
   * to this function with mac->data_out_left == 0.  This will send
   * a NULL packet, which indicates end-of-transfer.
   */
  mac->data_out_left -= 8;
  mac->data_out_max -= 8;
  mac->data_out += 8;

  if ((mac->data_out_left < 0) || (mac->data_out_max < 0)) {
    usb_mac_clear_tx(mac, 0);

    /* End of a MAC setup packet */
    if (mac->packet_type == packet_type_setup_out)
      mac->packet_type = packet_type_none;
    if (mac->packet_type == packet_type_setup_in)
      mac->packet_type = packet_type_none;
    if (mac->packet_type == packet_type_out)
      mac->packet_type = packet_type_none;
  }

  mac->packet_queued = 0;
}

/* Send data down the wire, interrupting any existing
 * data that may be queued.
 */
static int usb_mac_send_data(struct USBMAC *mac,
                             int epnum,
                             const void *data,
                             int count,
                             int max) {

  /* De-queue any data that may already be queued. */
  usb_mac_clear_tx(mac, 1);

  mac->data_out_epnum = epnum;
  mac->data_out_left = count;
  mac->data_out_max = max;
  mac->data_out = data;

  return 0;
}

int usbMacSendData(struct USBMAC *mac, int epnum, const void *data, int count) {

  int ret;

  if (mac->data_out || !mac->address || mac->packet_queued) {
    return -11; /* EAGAIN */
  }

  ret = usb_mac_send_data(mac, epnum, data, count, count);
  if (ret)
    return ret;

  usb_mac_process_tx(mac);

  (void) suspendThread(&mac->thread);

  return 0;
}

static int usb_mac_process_setup(struct USBMAC *mac, const uint8_t packet[10]) {

  const struct usb_mac_setup_packet *setup;
  const void *response = (void *)-1;
  uint32_t response_len = 0;
  struct USBLink *link = mac->link;

  setup = (const struct usb_mac_setup_packet *)packet;

  if ((setup->bmRequestType == 0x00) && (setup->bRequest == SET_ADDRESS)) {
    mac->address = setup->wValue;
  }
  else if ((setup->bmRequestType == 0x00) && (setup->bRequest == SET_CONFIGURATION)) {
    if (link->setConfigNum)
      link->setConfigNum(link, setup->wValue);
  }
  else {
    response_len = link->getDescriptor(link, setup, &response);
  }
  usb_mac_send_data(mac, mac->tok_epnum, response, response_len, setup->wLength);

  return 0;
}

static void usb_mac_parse_data(struct USBMAC *mac,
                               const uint8_t packet[10],
                               uint32_t count) {
  (void)count;

  switch (mac->packet_type) {

  case packet_type_setup:
    usb_mac_process_setup(mac, packet);
    usb_mac_process_tx(mac);
    mac->packet_type = packet_type_none;
    break;

  case packet_type_out:
    // XXX HACK: An OUT packet gets generated (on Windows at least) when
    // terminating a SETUP sequence.  This seems odd.
    if (mac->tok_epnum == 0)
      break;
    // Copy over the packet, minus the CRC16
    memcpy(mac->tok_buf + mac->tok_pos, packet, count - 2);
    mac->tok_pos += (count - 2);
    if (!mac->link->receiveData(mac->link, mac->tok_epnum, count - 2, packet))
      mac->packet_type = packet_type_none;
    break;

  case packet_type_in:
  case packet_type_none:
  default:
    break;
  }
}

static inline void usb_mac_parse_token(struct USBMAC *mac,
                                       const uint8_t packet[2]) {

  mac->tok_epnum = (((const uint16_t *)packet)[0] >> 7) & 0xf;
  mac->tok_addr  = (((const uint16_t *)packet)[0] >> 11) & 0x1f;
}

int usbMacProcess(struct USBMAC *mac,
                  const uint8_t packet[11],
                  uint32_t count) {

  switch(packet[0]) {
  case USB_PID_SETUP:
    mac->packet_type = packet_type_setup;
    usb_mac_clear_tx(mac, 1);
    usb_mac_parse_token(mac, packet + 1);
    break;

  case USB_PID_DATA0:
    mac->data_buffer |= (1 << mac->tok_epnum);
    usb_mac_parse_data(mac, packet + 1, count - 1);
    break;

  case USB_PID_DATA1:
    mac->data_buffer &= ~(1 << mac->tok_epnum);
    usb_mac_parse_data(mac, packet + 1, count - 1);
    break;

  case USB_PID_OUT:
    usb_mac_parse_token(mac, packet + 1);
    mac->packet_type = packet_type_out;
    mac->tok_pos = 0;
    mac->tok_buf = mac->link->getReceiveBuffer(mac->link, mac->tok_epnum, NULL);
  break;

  case USB_PID_ACK:
    mac->data_buffer ^= (1 << mac->tok_epnum);
    usbMacTransferSuccess(mac);
    if (mac->data_out) {
      usb_mac_process_tx(mac);
    }
    else {
      usb_mac_clear_tx(mac, 0);
    }
    break;

  default:
    break;
  }

  return 0;
}

void usbMacInit(struct USBMAC *mac, struct USBLink *link) {

  mac->link = link;
  usb_mac_clear_tx(mac, 1);
  mac->address = 0;
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

  return &default_mac;
}

void usbSendControl(void) {
  return;
}

void usbReceiveControl(void) {
  return;
}
