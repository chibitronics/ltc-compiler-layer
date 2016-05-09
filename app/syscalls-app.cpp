#include <stdint.h>
#include "Arduino.h"
extern "C" {
    __attribute__((naked))
    void __aeabi_memcpy8(void) {
      asm("svc #0");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memcpy4(void) {
      asm("svc #1");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memcpy(void) {
      asm("svc #2");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memmove8(void) {
      asm("svc #3");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memmove4(void) {
      asm("svc #4");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memmove(void) {
      asm("svc #5");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memset8(void) {
      asm("svc #6");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memset4(void) {
      asm("svc #7");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memset(void) {
      asm("svc #8");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memclr8(void) {
      asm("svc #9");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memclr4(void) {
      asm("svc #10");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_memclr(void) {
      asm("svc #11");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __eqdf2(void) {
      asm("svc #12");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __gedf2(void) {
      asm("svc #13");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __gtdf2(void) {
      asm("svc #14");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __ledf2(void) {
      asm("svc #15");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __ltdf2(void) {
      asm("svc #16");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __nedf2(void) {
      asm("svc #17");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __popcount_tab(void) {
      asm("svc #18");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __popcountsi2(void) {
      asm("svc #19");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_cdcmpeq(void) {
      asm("svc #20");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_cdcmple(void) {
      asm("svc #21");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_cdrcmple(void) {
      asm("svc #22");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __muldi3(void) {
      asm("svc #23");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_uidiv(void) {
      asm("svc #24");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_uidivmod(void) {
      asm("svc #25");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_uldivmod(void) {
      asm("svc #26");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_idiv(void) {
      asm("svc #27");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_idivmod(void) {
      asm("svc #28");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_lasr(void) {
      asm("svc #29");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_ldivmod(void) {
      asm("svc #30");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_llsl(void) {
      asm("svc #31");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_llsr(void) {
      asm("svc #32");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_lmul(void) {
      asm("svc #33");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void strcpy(void) {
      asm("svc #34");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void strlen(void) {
      asm("svc #35");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void realloc(void) {
      asm("svc #36");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __dso_handle(void) {
      asm("svc #37");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void __aeabi_atexit(void) {
      asm("svc #38");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void free(void) {
      asm("svc #39");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void memset(void) {
      asm("svc #40");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void ltoa(void) {
      asm("svc #41");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void utoa(void) {
      asm("svc #42");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void ultoa(void) {
      asm("svc #43");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void itoa(void) {
      asm("svc #44");
      asm("bx lr    ");
    }
    __attribute__((naked))
    void ledShow(void) {
      asm("svc #45");
      asm("bx lr    ");
    }
};
  __attribute__((naked))
  void pinMode(int pin, enum pin_mode mode) {
    asm("svc #46");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void digitalWrite(int pin, int value) {
    asm("svc #47");
    asm("bx lr    ");
  }
  __attribute__((naked))
  int digitalRead(int pin) {
    asm("svc #48");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void analogWrite(int pin, int value) {
    asm("svc #49");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void analogReference(enum analog_reference_type type) {
    asm("svc #50");
    asm("bx lr    ");
  }
  __attribute__((naked))
  int analogRead(int pin) {
    asm("svc #51");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void attachInterrupt(int irq, void (*func)(void), enum irq_mode mode) {
    asm("svc #52");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void detachInterrupt(int irq) {
    asm("svc #53");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void tone(int pin, unsigned int frequency, unsigned long duration) {
    asm("svc #54");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void noTone(int pin) {
    asm("svc #55");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void shiftOut(int dataPin, int clockPin, int bitOrder, uint8_t val) {
    asm("svc #56");
    asm("bx lr    ");
  }
  __attribute__((naked))
  uint8_t shiftIn(int dataPin, int clockPin, int bitOrder) {
    asm("svc #57");
    asm("bx lr    ");
  }
  __attribute__((naked))
  unsigned long pulseIn(int pin, uint8_t state, unsigned long timeout) {
    asm("svc #58");
    asm("bx lr    ");
  }
  __attribute__((naked))
  unsigned long pulseInLong(int pin, uint8_t state, unsigned long timeout) {
    asm("svc #59");
    asm("bx lr    ");
  }
  __attribute__((naked))
  unsigned long millis(void) {
    asm("svc #60");
    asm("bx lr    ");
  }
  __attribute__((naked))
  unsigned long micros(void) {
    asm("svc #61");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void delay(unsigned long msecs) {
    asm("svc #62");
    asm("bx lr    ");
  }
  __attribute__((naked))
  void delayMicroseconds(unsigned int usecs) {
    asm("svc #63");
    asm("bx lr    ");
  }
  __attribute__((naked))
  long map(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    asm("svc #64");
    asm("bx lr    ");
  }
  __attribute__((naked))
  long random(long min, long max) {
    asm("svc #65");
    asm("bx lr    ");
  }
  __attribute__((naked))
  long randomSeed(unsigned long seed) {
    asm("svc #66");
    asm("bx lr    ");
  }
