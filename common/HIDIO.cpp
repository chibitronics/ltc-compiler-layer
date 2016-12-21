/*
  Mouse.cpp

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#define DONT_DEFINE_HIDIO_OBJECT
#include "HIDIO.h"
#undef DONT_DEFINE_HIDIO_OBJECT

#if defined(_USING_HID)

#if 0
const uint8_t BOS_DESCRIPTOR_PREFIX[] PROGMEM = {
0x05,  // Length
0x0F,  // Binary Object Store descriptor
0x39, 0x00,  // Total length
0x02,  // Number of device capabilities

// WebUSB Platform Capability descriptor (bVendorCode == 0x01).
0x18,  // Length
0x10,  // Device Capability descriptor
0x05,  // Platform Capability descriptor
0x00,  // Reserved
0x38, 0xB6, 0x08, 0x34, 0xA9, 0x09, 0xA0, 0x47,
0x8B, 0xFD, 0xA0, 0x76, 0x88, 0x15, 0xB6, 0x65,  // WebUSB GUID
0x00, 0x01,  // Version 1.0
0x01,  // Vendor request code
};

// Landing page (1 byte) sent in the middle.

const uint8_t BOS_DESCRIPTOR_SUFFIX[] PROGMEM {
// Microsoft OS 2.0 Platform Capability Descriptor (MS_VendorCode == 0x02)
0x1C,  // Length
0x10,  // Device Capability descriptor
0x05,  // Platform Capability descriptor
0x00,  // Reserved
0xDF, 0x60, 0xDD, 0xD8, 0x89, 0x45, 0xC7, 0x4C,
0x9C, 0xD2, 0x65, 0x9D, 0x9E, 0x64, 0x8A, 0x9F,  // MS OS 2.0 GUID
0x00, 0x00, 0x03, 0x06,  // Windows version
0x2e, 0x00,  // Descriptor set length
0x02,  // Vendor request code
0x00   // Alternate enumeration code
};






const uint8_t MS_OS_20_DESCRIPTOR_PREFIX[] PROGMEM = {
// Microsoft OS 2.0 descriptor set header (table 10)
0x0A, 0x00,  // Descriptor size (10 bytes)
0x00, 0x00,  // MS OS 2.0 descriptor set header
0x00, 0x00, 0x03, 0x06,  // Windows version (8.1) (0x06030000)
0x2e, 0x00,  // Size, MS OS 2.0 descriptor set

// Microsoft OS 2.0 configuration subset header
0x08, 0x00,  // Descriptor size (8 bytes)
0x01, 0x00,  // MS OS 2.0 configuration subset header
0x00,        // bConfigurationValue
0x00,        // Reserved
0x24, 0x00,  // Size, M S OS 2.0 configuration subset

// Microsoft OS 2.0 function subset header
0x08, 0x00,  // Descriptor size (8 bytes)
0x02, 0x00,  // MS OS 2.0 function subset header
};

// First interface number (1 byte) sent here.

const uint8_t MS_OS_20_DESCRIPTOR_SUFFIX[] PROGMEM = {
0x00,        // Reserved
0x1c, 0x00,  // Size, MS OS 2.0 function subset

// Microsoft OS 2.0 compatible ID descriptor (table 13)
0x14, 0x00,  // wLength
0x03, 0x00,  // MS_OS_20_FEATURE_COMPATIBLE_ID
'W',  'I',  'N',  'U',  'S',  'B',  0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif

#if 0
static const uint8_t ms_os_20_descriptor_set[] = {//

0x0A, 0x00,                                    // Descriptor size (10 bytes)
0x00, 0x00,                                    // MS OS 2.0 descriptor set header
0x00, 0x00, 0x03, 0x06,                                        // Windows version (8.1) (0x06030000)
0x9E, 0x00,                                    // Size, MS OS 2.0 descriptor set (158 bytes)

// Microsoft OS 2.0 compatible ID descriptor

0x14, 0x00,                                            // Descriptor size (20 bytes)
0x03, 0x00,                                      // MS OS 2.0 compatible ID descriptor
0x57, 0x49, 0x4E, 0x55, 0x53, 0x42, 0x00, 0x00,                        // WINUSB string
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,                        // Sub-compatible ID

// Registry property descriptor

0x80, 0x00,                            // Descriptor size (130 bytes)
0x04, 0x00,                            // Registry Property descriptor
0x01, 0x00,                            // Strings are null-terminated Unicode
0x28, 0x00,                            // Size of Property Name (40 bytes)

//Property Name ("DeviceInterfaceGUID")

0x44, 0x00, 0x65, 0x00, 0x76, 0x00, 0x69, 0x00, 0x63, 0x00, 0x65, 0x00,
0x49, 0x00, 0x6E, 0x00, 0x74, 0x00, 0x65, 0x00, 0x72, 0x00, 0x66, 0x00,
0x61, 0x00, 0x63, 0x00, 0x65, 0x00, 0x47, 0x00, 0x55, 0x00, 0x49, 0x00,
0x44, 0x00, 0x00, 0x00,

0x4E, 0x00,                            // Size of Property Data (78 bytes)

// Vendor-defined Property Data: {ecceff35-146c-4ff3-acd9-8f992d09acdd}
/*
0x7b, 0x00, 0x64, 0x00, 0x65, 0x00, 0x65, 0x00, 0x38, 0x00, 0x32, 0x00,
0x34, 0x00, 0x65, 0x00, 0x66, 0x00, 0x2d, 0x00, 0x37, 0x00, 0x32, 0x00,
0x39, 0x00, 0x62, 0x00, 0x2d, 0x00, 0x34, 0x00, 0x61, 0x00, 0x30, 0x00,
0x65, 0x00, 0x2d, 0x00, 0x39, 0x00, 0x63, 0x00, 0x31, 0x00, 0x34, 0x00,
0x2d, 0x00, 0x62, 0x00, 0x37, 0x00, 0x31, 0x00, 0x31, 0x00, 0x37, 0x00,
0x64, 0x00, 0x33, 0x00, 0x33, 0x00, 0x61, 0x00, 0x38, 0x00, 0x31, 0x00,
0x37, 0x00, 0x7d, 0x00, 0x00, 0x00};
*/
0x7B, 0x00, 0x65, 0x00, 0x63, 0x00, 0x63, 0x00, 0x65, 0x00, 0x66, 0x00,
0x66, 0x00, 0x33, 0x00, 0x35, 0x00, 0x2D, 0x00, 0x31, 0x00, 0x34, 0x00,
0x36, 0x00, 0x33, 0x00, 0x2D, 0x00, 0x34, 0x00, 0x66, 0x00, 0x66, 0x00,
0x33, 0x00, 0x2D, 0x00, 0x61, 0x00, 0x63, 0x00, 0x64, 0x00, 0x39, 0x00,
0x2D, 0x00, 0x38, 0x00, 0x66, 0x00, 0x39, 0x00, 0x39, 0x00, 0x32, 0x00,
0x64, 0x00, 0x30, 0x00, 0x39, 0x00, 0x61, 0x00, 0x63, 0x00, 0x64, 0x00,
0x64, 0x00, 0x7D, 0x00, 0x00, 0x00};
#endif

#if 0
static const MicrosoftCompatIDDescriptor ms_os_20_descriptor_set = { sizeof(MicrosoftCompatIDDescriptor), 0x0100, 0x0004, 0x01, {0}, 0x00, 0x01, "WINUSB", {0}, {0} };
static const MicrosoftExtPropertiesDescriptor ms_os_20_ext_descriptor_set = { sizeof(MicrosoftExtPropertiesDescriptor), 0x0100, 0x0005, 0x0001,\
0x00000084, 0x00000001,\
0x0028,     {'D','e','v','i','c','e','I','n','t','e','r','f','a','c','e','G','U','I','D',0},\
0x0000004E, {'{','F','7','0','2','4','2','C','7','-','F','B','2','5','-','4','4','3','B', \
'-','9','E','7','E','-','A','4','2','6','0','F','3','7','3','9','8','2','}',0} };
#endif

#if 0
static const uint8_t ms_os_20_descriptor_set[] = {
  // Microsoft OS 2.0 descriptor set header (table 10)
  0x0A, 0x00,  // Descriptor size (10 bytes)
  0x00, 0x00,  // MS OS 2.0 descriptor set header
  0x00, 0x00, 0x03, 0x06,  // Windows version (8.1) (0x06030000)
  0x2e, 0x00,  // Size, MS OS 2.0 descriptor set

  // Microsoft OS 2.0 configuration subset header
  0x08, 0x00,  // Descriptor size (8 bytes)
  0x01, 0x00,  // MS OS 2.0 configuration subset header
  0x00,        // bConfigurationValue
  0x00,        // Reserved
  0x24, 0x00,  // Size, M S OS 2.0 configuration subset

  // Microsoft OS 2.0 function subset header
  0x08, 0x00,  // Descriptor size (8 bytes)
  0x02, 0x00,  // MS OS 2.0 function subset header
  0, // First interface number (1 byte) sent here.
  0x00,        // Reserved
  0x1c, 0x00,  // Size, MS OS 2.0 function subset

  // Microsoft OS 2.0 compatible ID descriptor (table 13)
  0x14, 0x00,  // wLength
  0x03, 0x00,  // MS_OS_20_FEATURE_COMPATIBLE_ID
  'W',  'I',  'N',  'U',  'S',  'B',  0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif

#if 1
#define MS_OS_20_SET_HEADER_DESCRIPTOR 0x00
#define MS_OS_20_SUBSET_HEADER_CONFIGURATION 0x01
#define MS_OS_20_SUBSET_HEADER_FUNCTION 0x02
#define MS_OS_20_FEATURE_COMPATIBLE_ID 0x03
#define MS_OS_20_FEATURE_REG_PROPERTY 0x04

#define MS_OS_20_DESCRIPTOR_LENGTH 0xc2

static const uint8_t ms_os_20_descriptor_set[] =
{
    // Microsoft OS 2.0 Descriptor Set header (Table 10)
    0x0A, 0x00,  // wLength
    MS_OS_20_SET_HEADER_DESCRIPTOR, 0x00,
    0x00, 0x00, 0x03, 0x06,  // dwWindowsVersion: Windows 8.1 (NTDDI_WINBLUE)
    MS_OS_20_DESCRIPTOR_LENGTH, 0x00,

    // Microsoft OS 2.0 registry property descriptor (Table 14)
    0x20, 0x00,   // wLength
    MS_OS_20_FEATURE_REG_PROPERTY, 0x00,
    0x07, 0x00,   // wPropertyDataType: REG_MULTI_SZ
    0x0C, 0x00,   // wPropertyNameLength
    'H',0,'a',0,'p',0,'p',0,'y',0,0,0,
    0x0A, 0x00,   // wPropertyDataLength
    '3',0,0,0,'5',0,0,0,0,0,

    // Microsoft OS 2.0 compatible ID descriptor (Table 13)
    0x14, 0x00,                                      // wLength
    MS_OS_20_FEATURE_COMPATIBLE_ID, 0x00,            // wDescriptorType
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,        // compatibleID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  // subCompatibleID

    // Microsoft OS 2.0 registry property descriptor (Table 14)
    0x84, 0x00,   // wLength
    MS_OS_20_FEATURE_REG_PROPERTY, 0x00,
    0x07, 0x00,   // wPropertyDataType: REG_MULTI_SZ
    0x2a, 0x00,   // wPropertyNameLength
    'D',0,'e',0,'v',0,'i',0,'c',0,'e',0,'I',0,'n',0,'t',0,'e',0,'r',0,
    'f',0,'a',0,'c',0,'e',0,'G',0,'U',0,'I',0,'D',0,'s',0,0,0,
    0x50, 0x00,   // wPropertyDataLength
    '{',0,'9',0,'9',0,'c',0,'4',0,'b',0,'b',0,'b',0,'0',0,'-',0,
    'e',0,'9',0,'2',0,'5',0,'-',0,'4',0,'3',0,'9',0,'7',0,'-',0,
    'a',0,'f',0,'e',0,'e',0,'-',0,'9',0,'8',0,'1',0,'c',0,'d',0,
    '0',0,'7',0,'0',0,'2',0,'1',0,'6',0,'3',0,'}',0,0,0,0,0,
};
#endif

#define USB_REPORT_ID 1
#define BUFFER_SIZE 8 /* After the USB_REPORT_ID, the packet has 7 bytes left */

static const uint8_t _hidReportDescriptor[] = {
  
  0x06, 0x00, 0xFF,       // USAGE_PAGE = 0xFF00 (Vendor Defined Page 1)
  0x09, 0x01,             // USAGE (Vendor Usage 1)
  0xA1, 0x01,             // COLLECTION (Application)
    0x15, 0x00,           //      USAGE_MINIMUM (data bytes in the report may have minimum value = 0x00)
    0x26, 0xFF, 0x00,     //      USAGE_MAXIMUM (data bytes in the report may have maximum value = 0x00FF = unsigned 255)

    0x75, 0x08,           //      REPORT_SIZE (8-bit field size)

    0x95, BUFFER_SIZE,    //      REPORT_COUNT Make eight 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
    0x09, 0x01,           //      USAGE (Undefined)
    0x81, 0x02,           //      INPUT (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.

    0x95, BUFFER_SIZE,    //      REPORT_COUNT (Make eight 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item))
    0x09, 0x01,           //      USAGE (Undefined)
    0x91, 0x00,           //      OUTPUT (Data, Array, Abs): Instantiates
                          //      output packet fields.  Uses same report size
                          //      and count as "Input" fields, since nothing
                          //      new/different was specified to the parser
                          //      since the "Input" item.

    0x95, BUFFER_SIZE,    //      REPORT_COUNT (Make eight 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item))
    0x09, 0x01,           //      USAGE (Undefined)
    0xB1, 0x02,           //      Feature: Data
  0xC0                    // END_COLLECTION
};

static uint8_t output_report[8];

int HIDIO_::SendBOSDescriptor(void) {

  static uint8_t bos_descriptor_buffer[sizeof(BOSDescriptor) + sizeof(WebUSBPlatformCapabilityDescriptor) + sizeof(MicrosoftOs2p0PlatformCapabilityDescriptor)];
  BOSDescriptor bos = D_BOS(sizeof(bos_descriptor_buffer), 2);
  WebUSBPlatformCapabilityDescriptor wusb = D_WEBUSB(1, landingPage);
  MicrosoftOs2p0PlatformCapabilityDescriptor msos2p0 = D_MSOS2p0(sizeof(ms_os_20_descriptor_set), 2);

  memcpy(bos_descriptor_buffer, &bos, sizeof(bos));
  memcpy(bos_descriptor_buffer + sizeof(bos), &wusb, sizeof(wusb));
  memcpy(bos_descriptor_buffer + sizeof(bos) + sizeof(wusb), &msos2p0, sizeof(msos2p0));

  USB_SendEntireControl(bos_descriptor_buffer, sizeof(bos_descriptor_buffer));
  return sizeof(bos_descriptor_buffer);
}

int HIDIO_::SendMSDescriptor(void) {

  USB_SendEntireControl(ms_os_20_descriptor_set, sizeof(ms_os_20_descriptor_set));
  return sizeof(ms_os_20_descriptor_set);
}

int HIDIO_::getDescriptor(USBSetup& setup)
{
  // Check if this is a HID Class Descriptor request
  if ((setup.bmRequestType == REQUEST_DEVICETOHOST_STANDARD_INTERFACE) 
   && (setup.wValueH == HID_REPORT_DESCRIPTOR_TYPE)
   && (setup.wIndex == pluggedInterface)) {
     USB_SendEntireControl(_hidReportDescriptor, sizeof(_hidReportDescriptor));
     return true;
  }
  else if (USB_BOS_DESCRIPTOR_TYPE == setup.wValueH)
  {
    if (setup.wValueL == 0 && setup.wIndex == 0) {
      return SendBOSDescriptor();
    }
  }
  return 0;
}

#define IN_EPNUM 1
#define OUT_EPNUM 2
int HIDIO_::getInterface(uint8_t* interfaceCount)
{
  *interfaceCount += 1; // uses 1
  WebUSBDescriptor webUSBInterface = {
    D_INTERFACE(pluggedInterface, 2, 0xff, 0, 0),
    //D_HIDREPORT(sizeof(_hidReportDescriptor)),
    D_ENDPOINT(USB_ENDPOINT_OUT(OUT_EPNUM), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 1),
    D_ENDPOINT(USB_ENDPOINT_IN(IN_EPNUM), USB_ENDPOINT_TYPE_INTERRUPT, USB_EP_SIZE, 1),
  };
  USB_SendControl(0, &webUSBInterface, sizeof(webUSBInterface));
  return true;
}

uint8_t HIDIO_::getShortName(char* name)
{
  memcpy(name, "LtC", 3);
  return 3;
}

bool HIDIO_::VendorControlRequest(USBSetup& setup)
{
  if (setup.bmRequestType == (REQUEST_DEVICETOHOST | REQUEST_VENDOR | REQUEST_DEVICE)) {
    if (setup.bRequest == 0x01 && setup.wIndex == WEBUSB_REQUEST_GET_ALLOWED_ORIGINS)
    {
      uint8_t allowedOriginsPrefix[] = {
        // Allowed Origins Header, bNumConfigurations = 1
        0x05, 0x00, (uint8_t)(0x0c + numAllowedOrigins), 0x00, 0x01,
        // Configuration Subset Header, bNumFunctions = 1
        0x04, 0x01, 0x01, 0x01,
        // Function Subset Header, bFirstInterface = pluggedInterface
        (uint8_t)(0x03 + numAllowedOrigins), 0x02, pluggedInterface
      };
      if (USB_SendControl(0, &allowedOriginsPrefix, sizeof(allowedOriginsPrefix)) < 0)
        return false;
      return USB_SendControl(0, allowedOrigins, numAllowedOrigins) >= 0;
    }
    else if (setup.bRequest == 0x01 && setup.wIndex == WEBUSB_REQUEST_GET_URL)
    {
                        if (setup.wValueL == 0 || setup.wValueL > numUrls)
        return false;
      const WebUSBURL& url = urls[setup.wValueL - 1];
      uint8_t urlLength = strlen(url.url);
      uint8_t descriptorLength = urlLength + 3;
      if (USB_SendControl(0, &descriptorLength, 1) < 0)
        return false;
      uint8_t descriptorType = 3;
      if (USB_SendControl(0, &descriptorType, 1) < 0)
        return false;
      if (USB_SendControl(0, &url.scheme, 1) < 0)
        return false;
      return USB_SendControl(0, url.url, urlLength) >= 0;
    }
    else if (setup.bRequest == 0x02 && setup.wIndex == MS_OS_20_REQUEST_DESCRIPTOR)
    {
      return SendMSDescriptor();
    }
  }
	return false;
}

bool HIDIO_::setup(USBSetup& setup)
{
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
      //protocol = setup.wValueL;
      return true;
    }
    if (request == HID_SET_IDLE) {
      //idle = setup.wValueL;
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
  if (REQUEST_VENDOR == (requestType & REQUEST_TYPE)) {
    return VendorControlRequest(setup);
  }
  return false;
}

//================================================================================

void HIDIO_::begin(void) 
{
}

void HIDIO_::end(void) 
{
}

/* Stream functions */
size_t HIDIO_::write(uint8_t byte)
{
  memset(output_report, 0, sizeof(output_report));
  output_report[0] = byte;
  return USB_Send(IN_EPNUM, output_report, sizeof(output_report));
}

size_t HIDIO_::write(void *buffer, int count)
{
  memset(output_report, 0, sizeof(output_report));
  memcpy(output_report, buffer, (count < (int)sizeof(output_report) ? count : sizeof(output_report)));
  return USB_Send(IN_EPNUM, output_report, sizeof(output_report));
}

int HIDIO_::available(void)
{
  return (_bufferOffset < _bufferSize) || USB_Available(OUT_EPNUM);
}

int HIDIO_::read(void)
{
  // If the buffer is empty, read in some USB data.
  // We need to ignore the report descriptor that lives at offset 0.
  while (_bufferOffset >= _bufferSize) {
    _bufferOffset = 1;
    _bufferSize = USB_RecvWait(OUT_EPNUM, _buffer, sizeof(_buffer));
  }

  return _buffer[_bufferOffset++];
}

size_t HIDIO_::readWait(void *buffer, int max)
{
  return USB_RecvWait(OUT_EPNUM, buffer, max);
}

int HIDIO_::peek(void)
{
  int val = read();
  _bufferOffset--;

  return val;
}

void HIDIO_::flush(void)
{
  return;
}


HIDIO_::HIDIO_(const WebUSBURL* urls, uint8_t numUrls, uint8_t landingPage,
               const uint8_t* allowedOrigins, uint8_t numAllowedOrigins)
  : PluggableUSBModule(2, 1, epType),
   urls(urls), numUrls(numUrls), landingPage(landingPage),
   allowedOrigins(allowedOrigins), numAllowedOrigins(numAllowedOrigins)
{
  // one interface, 2 endpoints
  epType[0] = EP_TYPE_INTERRUPT_OUT;
  epType[1] = EP_TYPE_INTERRUPT_IN;
  PluggableUSB().plug(this);
}
#endif
