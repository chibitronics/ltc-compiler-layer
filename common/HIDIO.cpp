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

#define USB_REPORT_ID 1
#define BUFFER_SIZE 8 /* After the USB_REPORT_ID, the packet has 7 bytes left */

static const uint8_t _hidReportDescriptor[] PROGMEM = {
  
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

//================================================================================

HIDIO_::HIDIO_(void) : _bufferSize(0), _bufferOffset(0)
{
    static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

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
  return HID().Send(output_report, sizeof(output_report));
}

size_t HIDIO_::write(void *buffer, int count)
{
  memset(output_report, 0, sizeof(output_report));
  memcpy(output_report, buffer, (count < sizeof(output_report) ? count : sizeof(output_report)));
  return HID().Send(output_report, sizeof(output_report));
}

int HIDIO_::available(void)
{
  return (_bufferOffset < _bufferSize) || HID().CanRecvReport(USB_REPORT_ID);
}

int HIDIO_::read(void)
{
  // If the buffer is empty, read in some USB data.
  // We need to ignore the report descriptor that lives at offset 0.
  while (_bufferOffset >= _bufferSize) {
    _bufferOffset = 1;
    _bufferSize = HID().RecvReportWait(USB_REPORT_ID, _buffer, sizeof(_buffer));
  }

  return _buffer[_bufferOffset++];
}

size_t HIDIO_::readWait(void *buffer, int max)
{
  return HID().RecvReportWait(USB_REPORT_ID, buffer, max);
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

#endif
