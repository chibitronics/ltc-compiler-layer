#include <stdlib.h>

void abort(void) {
  while(1);
}

void *malloc(size_t size) {
  return realloc(0, size);
}

int abs(int x) { return (x > 0) ? x : -x; }
