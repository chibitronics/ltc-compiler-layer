#include "Arduino.h"
#include "ChibiOS.h"

thread_t *createThreadFromHeap(size_t size,
                       tprio_t prio, tfunc_t pf, void *arg) {
  thread_t *thr = (thread_t *)malloc(THD_WORKING_AREA_SIZE(size));
  createThread((void *)thr, THD_WORKING_AREA_SIZE(size), prio, pf, arg);

  /* Mark thr->p_flags as CH_FLAG_MODE_HEAP, so ChibiOS will call free()
   * on the memory after it exits.
   */
  ((uint8_t *)thr)[0x1d] = 1;
  return thr;
}
