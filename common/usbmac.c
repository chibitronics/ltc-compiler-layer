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

static void usb_mac_process_data(struct USBMAC *mac) {

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
    mac->data_out = NULL;
    mac->packet_queued = 0;
    return;
  }

  /* Pick the correct PID, DATA0 or DATA1 */
  mac->data_buffer ^= (1 << mac->tok_epnum);
  if (mac->data_buffer & (1 << mac->tok_epnum))
    mac->packet.pid = USB_PID_DATA0;
  else
    mac->packet.pid = USB_PID_DATA1;

  /* If there's no data, prepare a special NULL packet */
  if ((mac->data_out_left == 0) || (mac->data_out_max == 0)) {

    /* The special-null thing only happens for EP0 */
    if (mac->data_out_epnum != 0) {
        mac->data_out = NULL;
        mac->packet_queued = 0;
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

void usbMacTransferSuccess(struct USBMAC *mac) {

  /* Reduce the amount of data left.
   * If the packet is divisible by 8, this will cause one more call
   * to this function with mac->data_out_left == 0.  This will send
   * a NULL packet, which indicates end-of-transfer.
   */
  mac->data_out_left -= 8;
  mac->data_out_max -= 8;
  mac->data_out += 8;

  if ((mac->data_out_left < 0) || (mac->data_out_max < 0)) {
    mac->data_out_left = 0;
    mac->data_out_max = 0;
    mac->data_out = NULL;

    /* End of a MAC setup packet */
    if (mac->packet_type == packet_type_setup_out)
      mac->packet_type = packet_type_none;
    if (mac->packet_type == packet_type_out)
      mac->packet_type = packet_type_none;
  }

  mac->packet_queued = 0;
}

static int usb_mac_send_data(struct USBMAC *mac,
                             int epnum,
                             const void *data,
                             int count,
                             int max) {

  /* De-queue any data that may already be queued. */
  mac->packet_queued = 0;
  mac->data_out_epnum = epnum;
  mac->data_out_left = count;
  mac->data_out_max = max;
  mac->data_out = data;

  if (mac->thread)
    resumeThread(&mac->thread, 1);

  return 0;
}

int usbMacSendData(struct USBMAC *mac, int epnum, const void *data, int count) {

  int ret;

  if (mac->data_out) {
    return -11; /* EAGAIN */
  }

  ret = usb_mac_send_data(mac, epnum, data, count, count);
  if (ret)
    return ret;

  usb_mac_process_data(mac);

  (void) suspendThread(&mac->thread);

  return 0;
}

static int usb_mac_process_setup_read(struct USBMAC *mac,
                                      const struct usb_mac_setup_packet *setup)
{
  const void *response = NULL;
  uint32_t len = 0;
  struct USBLink *link = mac->link;

  len = link->getDescriptor(link, setup, &response);
  usb_mac_send_data(mac, mac->tok_epnum, response, len, setup->wLength);

  return 0;
}

static int usb_mac_process_setup_write(struct USBMAC *mac,
                                       const struct usb_mac_setup_packet *setup)
{
  const void *response = (const void *)-1;
  int len = 0;
  int max = 0;
  int handled = 0;
  struct USBLink *link = mac->link;

  switch (setup->bmRequestType) {
  case 0x00:  /* Device-to-host, standard, write to host */
    switch (setup->bRequest) {
    case 5: /* SET_ADDRESS */
      mac->address = setup->wValue;
      handled = 1;
      break;

    case 9: /* SET_CONFIGURATION */
      if (link->setConfigNum)
        link->setConfigNum(link, setup->wValue);
      handled = 1;
      break;
    }
    break;
  }

  if (!handled) {
    if (link->getDescriptor)
      len = link->getDescriptor(link, setup, &response);
  }

  /* We must always send a response packet.  If there's ever a time when
   * we shouldn't send a packet, simply "return" rather than "break" above.
   */
  usb_mac_send_data(mac, 0, response, len, max);

  return 0;
}

static int usb_mac_process_setup(struct USBMAC *mac, const uint8_t packet[10]) {

  const struct usb_mac_setup_packet *setup;

  setup = (const struct usb_mac_setup_packet *)packet;

  if (setup->bmRequestType & 0x80) {
    /* Device-to-Host */
    mac->packet_type = packet_type_setup_in;
    return usb_mac_process_setup_read(mac, setup);
  }

  else {
    mac->packet_type = packet_type_setup_out;
    return usb_mac_process_setup_write(mac, setup);
  }

  return 0;
}

static inline void usb_mac_parse_token(struct USBMAC *mac,
                                       const uint8_t packet[2]) {

  mac->tok_epnum = (((const uint16_t *)packet)[0] >> 7) & 0xf;
  mac->tok_addr  = (((const uint16_t *)packet)[0] >> 11) & 0x1f;
}

static void usb_mac_parse_data(struct USBMAC *mac,
                               const uint8_t packet[10],
                               uint32_t count) {
  (void)count;

  switch (mac->packet_type) {

  case packet_type_setup:
    usb_mac_process_setup(mac, packet);
    break;

  case packet_type_in:
    /* If there is data, it will be fetched during parse_token */
    break;

  case packet_type_out:
    //mac->link->receiveData(mac->link, mac->tok_epnum, count, packet);
    memcpy(mac->tok_buf + mac->tok_pos, packet, count - 2);
    mac->tok_pos += (count - 2);
    if (!mac->link->receiveData(mac->link, mac->tok_epnum, count, packet))
      mac->packet_type = packet_type_none;
    break;

  case packet_type_none:
    break;

  default:
    break;
  }

  // The remote side sends an empty packet (with only a CRC16) to indicate END.
  if (count == 2) {
    mac->packet_type = packet_type_none;
//    mac->data_buffer = 0;
  }
}

int usbMacProcess(struct USBMAC *mac,
                  const uint8_t packet[11],
                  uint32_t count) {

  switch(packet[0]) {
  case USB_PID_SETUP:
    mac->packet_type = packet_type_setup;
    mac->data_buffer &= ~1;
    usb_mac_parse_token(mac, packet + 1);
    break;

  case USB_PID_DATA0:
    mac->data_buffer ^= (1 << mac->tok_epnum);;
    usb_mac_parse_data(mac, packet + 1, count - 1);
    break;

  case USB_PID_DATA1:
    mac->data_buffer ^= (1 << mac->tok_epnum);;
    usb_mac_parse_data(mac, packet + 1, count - 1);
    break;

  case USB_PID_OUT:
    if (mac->packet_type == packet_type_none) {
      //mac->data_buffer = 0;
      mac->packet_type = packet_type_out;
      usb_mac_parse_token(mac, packet + 1);
      mac->tok_pos = 0;
      mac->tok_buf = mac->link->getReceiveBuffer(mac->link, mac->tok_epnum, NULL);
    }
    break;

  case USB_PID_IN:
    if (mac->packet_type == packet_type_none) {
      //mac->data_buffer = 0;
      usb_mac_parse_token(mac, packet + 1);

      void *buffer;
      int32_t size;
      buffer = mac->link->getSendBuffer(mac->link, mac->tok_epnum, &size);

      if (buffer) {
        mac->packet_type = packet_type_in;
        usb_mac_send_data(mac, mac->tok_epnum, buffer, size, size);
      }
    }
    break;

  case USB_PID_ACK:
    if (!mac->data_out) {
      mac->data_out_left = 0;
      mac->data_out_max = 0;
      mac->data_out = NULL;
      mac->packet_type = packet_type_none;
      if (mac->thread)
        resumeThread(&mac->thread, 0);
    }
    break;

  default:
    break;
  }

  /* Pre-process any data that's remaining, so it can be sent out
   * in case an IN token comes.
   */
  usb_mac_process_data(mac);

  return 0;
}

void usbMacInit(struct USBMAC *mac, struct USBLink *link) {

  mac->link = link;
  mac->data_out_left = 0;
  mac->data_out_max = 0;
  mac->data_out = NULL;
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
