/*
   Copyright (c) 2015, Arduino LLC
   Original code (pre-library): Copyright (c) 2011, Peter Barrett

   Permission to use, copy, modify, and/or distribute this software for
   any purpose with or without fee is hereby granted, provided that the
   above copyright notice and this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
   WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
   BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
   OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
   WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
   ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
   SOFTWARE.
 */

#include "HID.h"

#if defined(USBCON)

HID_& HID()
{
	static HID_ obj;
	return obj;
}

int HID_::getInterface(uint8_t* interfaceCount)
{
	*interfaceCount += 1; // uses 1
	static HIDDescriptor hidInterface = {
		D_INTERFACE(pluggedInterface, 2, USB_DEVICE_CLASS_HUMAN_INTERFACE, HID_SUBCLASS_NONE, HID_PROTOCOL_NONE),
		D_HIDREPORT(descriptorSize),
		D_ENDPOINT(USB_ENDPOINT_IN(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 10),
		D_ENDPOINT(USB_ENDPOINT_OUT(pluggedEndpoint), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 10)
	};
	return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

int HID_::getDescriptor(USBSetup& setup)
{
	// Check if this is a HID Class Descriptor request
	if ((USB_BOS_DESCRIPTOR_TYPE == setup.wValueH) && (setup.wValueL == 0) && (setup.wIndex == 0)) {
		static BOSDescriptor bos = D_BOS(sizeof(BOSDescriptor), 0);
		USB_SendControl(0, &bos, sizeof(bos));
		return sizeof(bos);
	}
	else if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE) {
		return 0;
	}
	else if (setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE) {
		return 0;
	}

	// In a HID Class Descriptor wIndex cointains the interface number
	if (setup.wIndex != pluggedInterface) { return 0; }

	int total = 0;
	HIDSubDescriptor* node;
	for (node = rootNode; node; node = node->next) {
		int res = USB_SendControl(TRANSFER_PGM, node->data, node->length);
		if (res == -1)
			return -1;
		total += res;
	}

	// Reset the protocol on reenumeration. Normally the host should not assume the state of the protocol
	// due to the USB specs, but Windows and Linux just assumes its in report mode.
	protocol = HID_REPORT_PROTOCOL;

	return total;
}

uint8_t HID_::getShortName(char *name)
{
	name[0] = 'H';
	name[1] = 'I';
	name[2] = 'D';
	name[3] = 'A' + (descriptorSize & 0x0F);
	name[4] = 'A' + ((descriptorSize >> 4) & 0x0F);
	return 5;
}

void HID_::AppendDescriptor(HIDSubDescriptor *node)
{
	if (!rootNode) {
		rootNode = node;
	} else {
		HIDSubDescriptor *current = rootNode;
		while (current->next) {
			current = current->next;
		}
		current->next = node;
	}
	descriptorSize += node->length;
}

int HID_::SendReport(uint8_t id, const void *data, int len)
{

  /* Prefix the report with the report ID */
  uint8_t packet[len + 1];
  packet[0] = id;
  memcpy(packet + 1, data, len);
  return USB_Send(pluggedEndpoint, packet, sizeof(packet));
}

int HID_::Send(const void *data, int len)
{
  /* Prefix the report with the report ID */
  return USB_Send(pluggedEndpoint, data, len);
}

int HID_::RecvReport(uint8_t id, void *data, int len)
{
  return USB_Recv(pluggedEndpoint, data, len);
}

int HID_::RecvReportWait(uint8_t id, void *data, int len)
{
  return USB_RecvWait(pluggedEndpoint, data, len);
}

int HID_::CanRecvReport(uint8_t id)
{
  return USB_Available(pluggedEndpoint);
}

bool HID_::setup(USBSetup& setup)
{
	if (pluggedInterface != setup.wIndex) {
		return false;
	}

	uint8_t request = setup.bRequest;
	uint8_t requestType = setup.bmRequestType;

	if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE)
	{
		if (request == HID_GET_REPORT) {
			// TODO: HID_GetReport();
			return true;
		}
		if (request == HID_GET_PROTOCOL) {
			// TODO: Send8(protocol);
			return true;
		}
		if (request == HID_GET_IDLE) {
			// TODO: Send8(idle);
		}
	}

	if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE)
	{
		if (request == HID_SET_PROTOCOL) {
			// The USB Host tells us if we are in boot or report mode.
			// This only works with a real boot compatible device.
			protocol = setup.wValueL;
			return true;
		}
		if (request == HID_SET_IDLE) {
			idle = setup.wValueL;
			return true;
		}
		if (request == HID_SET_REPORT)
		{
			//uint8_t reportID = setup.wValueL;
			//uint16_t length = setup.wLength;
			//uint8_t data[length];
			// Make sure to not read more data than USB_EP_SIZE.
			// You can read multiple times through a loop.
			// The first byte (may!) contain the reportID on a multreport.
			//USB_RecvControl(data, length);
		}
	}

	return false;
}

HID_::HID_(void) : PluggableUSBModule(2, 1, epType),
                   rootNode(NULL), descriptorSize(0),
                   protocol(HID_REPORT_PROTOCOL), idle(1)
{
	epType[0] = EP_TYPE_INTERRUPT_IN;
	epType[1] = EP_TYPE_INTERRUPT_OUT;
	PluggableUSB().plug(this);
}

int HID_::begin(void)
{
	return 0;
}

#endif /* if defined(USBCON) */
