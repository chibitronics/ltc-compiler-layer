/*
  Mouse.h

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

#ifndef HIDIO_h
#define HIDIO_h

#include "HID.h"

class HIDIO_ : public Stream
{
private:

  /* When calling the 1-byte HIDIO_::read(), buffer in some data.
   * Store this data in _buffer[], and keep peeling off bits every
   * time ::read() is called.  Keep track of how far into the buffer
   * we are by looking at _bufferOffset.
   */
  uint8_t _buffer[8];
  int _bufferSize;
  int _bufferOffset;

public:
  HIDIO_(void);
  void begin(void);
  void end(void);
  size_t write(void *buffer, int count);
  size_t readWait(void *buffer, int count);

  /* Stream class implementations */
  size_t write(uint8_t);
  int available(void);
  int read(void);
  int peek(void);
  void flush(void);
};

#ifdef DONT_DEFINE_HIDIO_OBJECT
extern
#endif
HIDIO_ HIDIO;

#endif /* HIDIO */
