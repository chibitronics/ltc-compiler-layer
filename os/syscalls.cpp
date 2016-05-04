#include "Arduino.h"

extern "C" {
  extern uint32_t __aeabi_unwind_cpp_pr0;
  extern uint32_t __aeabi_unwind_cpp_pr1;
};

uint32_t *SysCall_Table[] = {
  0,
  (uint32_t *)&__aeabi_unwind_cpp_pr0,
  (uint32_t *)&__aeabi_unwind_cpp_pr1,
  (uint32_t *)static_cast<void (*)(int, enum pin_mode)>(&pinMode),
  (uint32_t *)static_cast<void (*)(int, int)>(&digitalWrite),
  (uint32_t *)static_cast<int (*)(int)>(&digitalRead),
  (uint32_t *)static_cast<void (*)(unsigned long)>(&delay),
};
