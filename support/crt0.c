#include <stdlib.h>
#include <stdint.h>

void abort(void) {
  while(1);
}

#ifdef abs
#undef abs
#endif
int abs(int x) { return (x > 0) ? x : -x; }

void raise(void) {
  while(1);
}

// handle for C++ destructors, which we don't use.
// Taken from https://lists.debian.org/debian-gcc/2003/07/msg00070.html
void*   __dso_handle = (void*) &__dso_handle;
