#include "Arduino.h"

extern "C" long random_c(long min, long max);

long random(long min, long max) {
  return random_c(min, max);
}
