#include <stdlib.h>

void abort(void) {
  while(1);
}

int abs(int x) { return (x > 0) ? x : -x; }

void raise(void) {
  while(1);
}
