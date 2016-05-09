#include <stdlib.h>

void abort(void) {
  while(1);
}

void *malloc(size_t size) {
  return realloc(0, size);
}
