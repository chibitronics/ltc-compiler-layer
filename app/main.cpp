#include <stdint.h>

#include "Arduino.h"

void setup(void) {
  pinMode(32 + 6, OUTPUT);
  pinMode(32 + 7, OUTPUT);
  pinMode(32 + 10, OUTPUT);
}

void loop(void) {
  static int loop = 0;

  loop++;

  digitalWrite(32 + 10, loop & 1);
  digitalWrite(32 + 7, loop & 2);
  digitalWrite(32 + 6, loop & 4);

  delay(500);
}
