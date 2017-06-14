#include "Arduino.h"

extern "C" void delay_c(unsigned long msecs);
extern "C" void delayMicroseconds_c(unsigned int usecs);

void delay(unsigned long msecs) {
  delay_c(msecs);
}

void delayMicroseconds(unsigned int usecs) {
  if (!usecs)
    return;
  delayMicroseconds_c(usecs);
}
