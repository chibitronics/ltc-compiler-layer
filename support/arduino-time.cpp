#include "Arduino.h"

// The OS has a bug where sleeping for 0 causes a lockup.
// Simply add 1 to the value to work around this.
__attribute__((naked))
void delay(unsigned long msecs) {
  asm("add r0, #1");
  asm("svc #106");
}

__attribute__((naked))
void delayMicroseconds(unsigned int usecs) {
  asm("add r0, #1");
  asm("svc #107");
}
