
void _kill(void) {
  while(1);
}

int _getpid(void) {
  return 0;
}

void _sbrk(void) {
  return;
}

void _exit(void) {
  while(1);
}

int __errno;

void abort(void) {
  while(1);
}

void __aeabi_unwind_cpp_pr0(void) {
  abort();
}

void __aeabi_unwind_cpp_pr1(void) {
  abort();
}

void __aeabi_unwind_cpp_pr2(void) {
  abort();
}

void __aeabi_atexit(void) {
  return;
}
