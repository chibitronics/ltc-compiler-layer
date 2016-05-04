#include "Arduino.h"

extern "C" {
  void __aeabi_unwind_cpp_pr0(void) {
    asm("svc #1");
  }
  
  void __aeabi_unwind_cpp_pr1(void) {
    asm("svc #2");
  }
};

  void pinMode(int pin, enum pin_mode mode) {
    asm("svc #3");
  }

  void digitalWrite(int pin, int value) {
    asm("svc #4");
  }

  int digitalRead(int pin) {
    asm("svc #5");
  }

  void delay(unsigned long delay) {
    asm("svc #6");
  }
