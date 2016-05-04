#include "Arduino.h"

uint32_t *SysCall_Table[] = {
  0,
  0,
  0,
  (uint32_t *)static_cast<void (*)(int, enum pin_mode)>(&pinMode),
  (uint32_t *)static_cast<void (*)(int, int)>(&digitalWrite),
  (uint32_t *)static_cast<int (*)(int)>(&digitalRead),
  (uint32_t *)static_cast<void (*)(unsigned long)>(&delay),
};
