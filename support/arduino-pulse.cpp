#include "Arduino.h"

unsigned long pulseIn(int pin, uint8_t state, unsigned long timeout) {
  uint32_t startMicros = micros();
  state = !!state;

  /* Wait for previous pulse to end */
  while (digitalRead(pin) == state)
    if (micros() - startMicros > timeout)
      return 0;

  /* Wait for the pulse to start */
  while (digitalRead(pin) != state)
    if (micros() - startMicros > timeout)
      return 0;

  /* Wait for the pulse to end */
  while (digitalRead(pin) == state)
    if (micros() - startMicros > timeout)
      return 0;

  return micros() - startMicros;
}

unsigned long pulseInLong(int pin, uint8_t state, unsigned long timeout) {
  return pulseIn(pin, state, timeout);
}
