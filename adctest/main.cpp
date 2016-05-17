#include "Arduino.h"

#define PIN (32 + 10)

void setup(void) {
  printf("Setting up pin %d\r\n", PIN);
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN, LOW);
}

void loop(void) {
  printf("Read from pin: %d\r\n", analogRead(PIN));
  digitalWrite(PIN, LOW);
  delay(100);
  digitalWrite(PIN, HIGH);
  delayMicroseconds(500000);
}
