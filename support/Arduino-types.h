#ifndef __ARDUINO_TYPES_KOSAGI_H__
#define __ARDUINO_TYPES_KOSAGI_H__

#include <stdint.h>
#include <stdbool.h>

#ifndef boolean
#define boolean bool
#endif

#ifndef byte
#define byte uint8_t
#endif

#ifndef NULL
#define NULL 0
#endif

enum analog_reference_type {
  DEFAULT = 0,
  INTERNAL = 1,
  INTERNAL1V1 = 2,
  INTERNAL2V56 = 3,
  EXTERNAL = 4,
};

enum irq_mode {
  LOW = 0,
  HIGH = 1,
  CHANGE = 2,
  RISING = 3,
  FALLING = 4,
};

enum pin_mode {
  INPUT = 0,
  OUTPUT = 1,
  INPUT_PULLUP = 2,
  INPUT_PULLDOWN = 3,
  OUTPUT_LOW = 0x21,
};

#define LSBFIRST 0
#define MSBFIRST 1

#endif /* __ARDUINO_TYPES_KOSAGI_H__ */
