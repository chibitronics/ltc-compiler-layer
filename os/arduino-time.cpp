#include "kl02.h"
#include "Arduino.h"

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long msecs) {
  int i;
  for (i = 0; i < 1000000; i++)
    asm("nop");
}
void delayMicroseconds(unsigned int usecs);

