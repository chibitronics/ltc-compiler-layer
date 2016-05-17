#include <stdint.h>
#include "Arduino-types.h"
extern "C" {
    __attribute__((naked))
    void __aeabi_atexit(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __cxa_atexit(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void atexit(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void _atexit(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __cxa_finalize(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2f(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2d(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_h2f(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_h2f_alt(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2h(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2h_alt(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2h(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2h_alt(void) {
      asm("svc #0");
      asm("bx lr");
    }
    __attribute__((naked))
    void memcpy(void) {
      asm("svc #1");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memcpy8(void) {
      asm("svc #1");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memcpy4(void) {
      asm("svc #1");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memcpy(void) {
      asm("svc #1");
      asm("bx lr");
    }
    __attribute__((naked))
    void memmove(void) {
      asm("svc #2");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memmove8(void) {
      asm("svc #2");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memmove4(void) {
      asm("svc #2");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memmove(void) {
      asm("svc #2");
      asm("bx lr");
    }
    __attribute__((naked))
    void memset(void) {
      asm("svc #3");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memset8(void) {
      asm("svc #3");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memset4(void) {
      asm("svc #3");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memset(void) {
      asm("svc #3");
      asm("bx lr");
    }
    __attribute__((naked))
    void memclr(void) {
      asm("svc #4");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memclr8(void) {
      asm("svc #4");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memclr4(void) {
      asm("svc #4");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_memclr(void) {
      asm("svc #4");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_uread4(void) {
      asm("svc #5");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_uwrite4(void) {
      asm("svc #6");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_uread8(void) {
      asm("svc #7");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_idiv(void) {
      asm("svc #8");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_uidiv(void) {
      asm("svc #9");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_idivmod(void) {
      asm("svc #10");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_uidivmod(void) {
      asm("svc #11");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_lmul(void) {
      asm("svc #12");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_ldivmod(void) {
      asm("svc #13");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_uldivmod(void) {
      asm("svc #14");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_llsl(void) {
      asm("svc #15");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_llsr(void) {
      asm("svc #16");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_lasr(void) {
      asm("svc #17");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_lcmp(void) {
      asm("svc #18");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_ulcmp(void) {
      asm("svc #19");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_i2f(void) {
      asm("svc #20");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_l2f(void) {
      asm("svc #20");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_i2d(void) {
      asm("svc #21");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_l2d(void) {
      asm("svc #21");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_ui2f(void) {
      asm("svc #22");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_ul2f(void) {
      asm("svc #22");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_ui2d(void) {
      asm("svc #23");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_ul2d(void) {
      asm("svc #23");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2iz(void) {
      asm("svc #24");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2lz(void) {
      asm("svc #24");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2iz(void) {
      asm("svc #25");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2lz(void) {
      asm("svc #25");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2uiz(void) {
      asm("svc #26");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_f2ulz(void) {
      asm("svc #26");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2uiz(void) {
      asm("svc #27");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_d2ulz(void) {
      asm("svc #27");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_cfcmpeq(void) {
      asm("svc #28");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_cdcmpeq(void) {
      asm("svc #29");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_cfcmple(void) {
      asm("svc #30");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_cdcmple(void) {
      asm("svc #31");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_cfrcmple(void) {
      asm("svc #32");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_cdcrmple(void) {
      asm("svc #33");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fcmpeq(void) {
      asm("svc #34");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dcmpeq(void) {
      asm("svc #35");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fcmplt(void) {
      asm("svc #36");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dcmplt(void) {
      asm("svc #37");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fcmple(void) {
      asm("svc #38");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dcmple(void) {
      asm("svc #39");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fcmpge(void) {
      asm("svc #40");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dcmpge(void) {
      asm("svc #41");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fcmpgt(void) {
      asm("svc #42");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dcmpgt(void) {
      asm("svc #43");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fcmpun(void) {
      asm("svc #44");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dcmpun(void) {
      asm("svc #45");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fadd(void) {
      asm("svc #46");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dadd(void) {
      asm("svc #47");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fdiv(void) {
      asm("svc #48");
      asm("bx lr");
    }
    __attribute__((naked))
    void __adabi_ddiv(void) {
      asm("svc #49");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fmul(void) {
      asm("svc #50");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dmul(void) {
      asm("svc #51");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_frsub(void) {
      asm("svc #52");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_drsub(void) {
      asm("svc #53");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_fsub(void) {
      asm("svc #54");
      asm("bx lr");
    }
    __attribute__((naked))
    void __aeabi_dsub(void) {
      asm("svc #55");
      asm("bx lr");
    }
    __attribute__((naked))
    void cos(void) {
      asm("svc #56");
      asm("bx lr");
    }
    __attribute__((naked))
    void cosf(void) {
      asm("svc #56");
      asm("bx lr");
    }
    __attribute__((naked))
    void sin(void) {
      asm("svc #57");
      asm("bx lr");
    }
    __attribute__((naked))
    void sinf(void) {
      asm("svc #57");
      asm("bx lr");
    }
    __attribute__((naked))
    void tan(void) {
      asm("svc #58");
      asm("bx lr");
    }
    __attribute__((naked))
    void tanf(void) {
      asm("svc #58");
      asm("bx lr");
    }
    __attribute__((naked))
    void atan2(void) {
      asm("svc #59");
      asm("bx lr");
    }
    __attribute__((naked))
    void atan2f(void) {
      asm("svc #59");
      asm("bx lr");
    }
    __attribute__((naked))
    void exp(void) {
      asm("svc #60");
      asm("bx lr");
    }
    __attribute__((naked))
    void fexp(void) {
      asm("svc #60");
      asm("bx lr");
    }
    __attribute__((naked))
    void log2(void) {
      asm("svc #61");
      asm("bx lr");
    }
    __attribute__((naked))
    void log2f(void) {
      asm("svc #61");
      asm("bx lr");
    }
    __attribute__((naked))
    void sqrt(void) {
      asm("svc #62");
      asm("bx lr");
    }
    __attribute__((naked))
    void sqrtf(void) {
      asm("svc #62");
      asm("bx lr");
    }
    __attribute__((naked))
    void strncpy(void) {
      asm("svc #63");
      asm("bx lr");
    }
    __attribute__((naked))
    void strcpy(void) {
      asm("svc #64");
      asm("bx lr");
    }
    __attribute__((naked))
    void strcmp(void) {
      asm("svc #65");
      asm("bx lr");
    }
    __attribute__((naked))
    void strncmp(void) {
      asm("svc #66");
      asm("bx lr");
    }
    __attribute__((naked))
    void strchr(void) {
      asm("svc #67");
      asm("bx lr");
    }
    __attribute__((naked))
    void strlen(void) {
      asm("svc #68");
      asm("bx lr");
    }
    __attribute__((naked))
    void snprintf(void) {
      asm("svc #69");
      asm("bx lr");
    }
    __attribute__((naked))
    void ltoa(void) {
      asm("svc #70");
      asm("bx lr");
    }
    __attribute__((naked))
    void utoa(void) {
      asm("svc #71");
      asm("bx lr");
    }
    __attribute__((naked))
    void ultoa(void) {
      asm("svc #72");
      asm("bx lr");
    }
    __attribute__((naked))
    void itoa(void) {
      asm("svc #73");
      asm("bx lr");
    }
    __attribute__((naked))
    void strtol(void) {
      asm("svc #74");
      asm("bx lr");
    }
    __attribute__((naked))
    void strtoul(void) {
      asm("svc #75");
      asm("bx lr");
    }
    __attribute__((naked))
    void printf(void) {
      asm("svc #76");
      asm("bx lr");
    }
    __attribute__((naked))
    void tfp_printf(void) {
      asm("svc #76");
      asm("bx lr");
    }
    __attribute__((naked))
    void putchar(void) {
      asm("svc #77");
      asm("bx lr");
    }
    __attribute__((naked))
    void getchar(void) {
      asm("svc #78");
      asm("bx lr");
    }
    __attribute__((naked))
    void cangetchar(void) {
      asm("svc #79");
      asm("bx lr");
    }
    __attribute__((naked))
    void free(void) {
      asm("svc #80");
      asm("bx lr");
    }
    __attribute__((naked))
    void malloc(void) {
      asm("svc #81");
      asm("bx lr");
    }
    __attribute__((naked))
    void realloc(void) {
      asm("svc #82");
      asm("bx lr");
    }
    __attribute__((naked))
    void __dso_handle(void) {
      asm("svc #83");
      asm("bx lr");
    }
    __attribute__((naked))
    void ledShow(void) {
      asm("svc #84");
      asm("bx lr");
    }
};
  __attribute__((naked))
  void pinMode(int pin, enum pin_mode mode) {
    asm("svc #85");
    asm("bx lr");
  }
  __attribute__((naked))
  void digitalWrite(int pin, int value) {
    asm("svc #86");
    asm("bx lr");
  }
  __attribute__((naked))
  int digitalRead(int pin) {
    asm("svc #87");
    asm("bx lr");
  }
  __attribute__((naked))
  void analogWrite(int pin, int value) {
    asm("svc #88");
    asm("bx lr");
  }
  __attribute__((naked))
  void analogReference(enum analog_reference_type type) {
    asm("svc #89");
    asm("bx lr");
  }
  __attribute__((naked))
  int analogRead(int pin) {
    asm("svc #90");
    asm("bx lr");
  }
  __attribute__((naked))
  void attachInterrupt(int irq, void (*func)(void), enum irq_mode mode) {
    asm("svc #91");
    asm("bx lr");
  }
  __attribute__((naked))
  void detachInterrupt(int irq) {
    asm("svc #92");
    asm("bx lr");
  }
  __attribute__((naked))
  void tone(int pin, unsigned int frequency, unsigned long duration) {
    asm("svc #93");
    asm("bx lr");
  }
  __attribute__((naked))
  void noTone(int pin) {
    asm("svc #94");
    asm("bx lr");
  }
  __attribute__((naked))
  void shiftOut(int dataPin, int clockPin, int bitOrder, uint8_t val) {
    asm("svc #95");
    asm("bx lr");
  }
  __attribute__((naked))
  uint8_t shiftIn(int dataPin, int clockPin, int bitOrder) {
    asm("svc #96");
    asm("bx lr");
  }
  __attribute__((naked))
  unsigned long pulseIn(int pin, uint8_t state, unsigned long timeout) {
    asm("svc #97");
    asm("bx lr");
  }
  __attribute__((naked))
  unsigned long pulseInLong(int pin, uint8_t state, unsigned long timeout) {
    asm("svc #98");
    asm("bx lr");
  }
  __attribute__((naked))
  unsigned long millis(void) {
    asm("svc #99");
    asm("bx lr");
  }
  __attribute__((naked))
  unsigned long micros(void) {
    asm("svc #100");
    asm("bx lr");
  }
  __attribute__((naked))
  void delay(unsigned long msecs) {
    asm("svc #101");
    asm("bx lr");
  }
  __attribute__((naked))
  void delayMicroseconds(unsigned int usecs) {
    asm("svc #102");
    asm("bx lr");
  }
  __attribute__((naked))
  long map(long value, long fromLow, long fromHigh, long toLow, long toHigh) {
    asm("svc #103");
    asm("bx lr");
  }
  __attribute__((naked))
  long random(long min, long max) {
    asm("svc #104");
    asm("bx lr");
  }
  __attribute__((naked))
  long randomSeed(unsigned long seed) {
    asm("svc #105");
    asm("bx lr");
  }
