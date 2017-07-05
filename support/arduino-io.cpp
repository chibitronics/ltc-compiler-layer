#include "Arduino.h"

extern "C" int digitalRead_c(int pin);

int digitalRead(int pin) {
  return digitalRead_c(pin) ? HIGH : LOW;
}
