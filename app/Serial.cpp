#include "Serial.h"

HardwareSerial Serial;

HardwareSerial::HardwareSerial() {
}

void HardwareSerial::begin(unsigned long a, uint8_t b) {
  return;
}

void HardwareSerial::end(void) {
  return;
}

int HardwareSerial::available(void) {
  return 0;
}

int HardwareSerial::peek(void) {
  return 0;
}

int HardwareSerial::read(void) {
  return 0;
}

int HardwareSerial::availableForWrite(void) {
  return 0;
}

void HardwareSerial::flush(void) {
  return;
}

size_t HardwareSerial::write(uint8_t) {
  return 0;
}
