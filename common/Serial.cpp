#include "Arduino.h"
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

  return cangetchar();
}

int HardwareSerial::peek(void) {

#warning "Implement peek"
  return 0;
}

int HardwareSerial::read(void) {

  return getchar();
}

int HardwareSerial::availableForWrite(void) {

  return 1;
}

void HardwareSerial::flush(void) {
  return;
}

size_t HardwareSerial::write(uint8_t c) {

  return putchar(c);
}
