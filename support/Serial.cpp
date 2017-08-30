#include "Arduino.h"
#include "Serial.h"
#include "ChibiOS.h"

HardwareSerial Serial;

void HardwareSerial::begin(unsigned long speed, uint8_t flags) {

  (void)flags;
  setSerialSpeed(speed);
  return;
}

void HardwareSerial::end(void) {
  return;
}

int HardwareSerial::available(void) {

  return serialCanGetChar();
}

int HardwareSerial::peek(void) {

  if (have_next_byte)
    return next_byte;
  if (serialCanGetChar()) {
    have_next_byte = 1;
    next_byte = serialGetChar();
    return next_byte;
  }
  return 0;
}

int HardwareSerial::read(void) {

  if (have_next_byte) {
    have_next_byte = 0;
    return next_byte;
  }
  return serialGetChar();
}

int HardwareSerial::availableForWrite(void) {

  return 1;
}

void HardwareSerial::flush(void) {
  return;
}

size_t HardwareSerial::write(uint8_t c) {

  return serialPutChar(c);
}

// In order to prune out the HardwareSerial object when Serial
// isn't used, we have a macro inside Serial.h that translates
// all instances of "Serial" to (*(serialWrapper())).  That way,
// if "Serial" is not used at all, the HardwareSerial object
// will get optimized away.
// This is to work around a C++ feature that says global objects
// are always created.  We tried to use LTO to optimize away
// the object, but couldn't get it to work with GCC 4.8 as used
// by the Arduino project.
HardwareSerial *serialWrapper(void) {
  static HardwareSerial serial;
  return &serial;
}