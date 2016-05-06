#include <stdint.h>

#include "Arduino.h"

#define RED 32 + 6
#define GREEN 32 + 7
#define BLUE 32 + 10

void setup(void) {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop(void) {
  static int loop = 0;

  loop++;

  digitalWrite(RED, loop & 1);
  digitalWrite(GREEN, loop & 2);
  digitalWrite(BLUE, loop & 4);

  delay(500);
}
