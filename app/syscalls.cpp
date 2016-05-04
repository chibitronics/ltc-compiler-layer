#include "Arduino.h"

extern "C" {
};

  void pinMode(int pin, enum pin_mode mode) {
    asm("svc #34");
  }

  void digitalWrite(int pin, int value) {
    asm("svc #35");
  }

  int digitalRead(int pin) {
    asm("svc #36");
  }

  void delay(unsigned long delay) {
    asm("svc #50");
  }
