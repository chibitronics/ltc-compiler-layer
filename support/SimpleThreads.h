//// v1 of Jie's API for the LTC simplified threads

#ifndef __LTC_SIMPLETHREADS_H__
#define __LTC_SIMPLETHREADS_H__

#define THREAD_MEMORY 64
#define THREAD_PRIORITY 20

void thread0_master(void *config);
void thread1_master(void *config);
void thread2_master(void *config);
void thread3_master(void *config);
void thread4_master(void *config);
void thread5_master(void *config);

void setup0(void);
void setup1(void);
void setup2(void);
void setup3(void);
void setup4(void);
void setup5(void);

void loop0(void);
void loop1(void);
void loop2(void);
void loop3(void);
void loop4(void);
void loop5(void);

void setup(void) {
  createThreadFromHeap(THD_WORKING_AREA_SIZE(THREAD_MEMORY), THREAD_PRIORITY,
                       thread0_master, NULL);

  createThreadFromHeap(THD_WORKING_AREA_SIZE(THREAD_MEMORY), THREAD_PRIORITY,
                       thread1_master, NULL);

  createThreadFromHeap(THD_WORKING_AREA_SIZE(THREAD_MEMORY), THREAD_PRIORITY,
                       thread2_master, NULL);

  createThreadFromHeap(THD_WORKING_AREA_SIZE(THREAD_MEMORY), THREAD_PRIORITY,
                       thread3_master, NULL);

  createThreadFromHeap(THD_WORKING_AREA_SIZE(THREAD_MEMORY), THREAD_PRIORITY,
                       thread4_master, NULL);

  createThreadFromHeap(THD_WORKING_AREA_SIZE(THREAD_MEMORY), THREAD_PRIORITY,
                       thread5_master, NULL);
}

void loop(void) {
  // since we already started two threads, the "main" thread of the program
  // is no longer needed. We exit the thread, which quits the loop and frees
  // up the Chibi Chip to focus on your new threads!
  exitThread(0);
}

void thread0_master(void *config) {
  setup0();
  while(1) {
    loop0();
    yieldThread();
  }
}

void thread1_master(void *config) {
  setup1();
  while(1) {
    loop1();
    yieldThread();
  }
}

void thread2_master(void *config) {
  setup2();
  while(1) {
    loop2();
    yieldThread();
  }
}

void thread3_master(void *config) {
  setup3();
  while(1) {
    loop3();
    yieldThread();
  }
}

void thread4_master(void *config) {
  setup4();
  while(1) {
    loop4();
    yieldThread();
  }
}

void thread5_master(void *config) {
  setup5();
  while(1) {
    loop5();
    yieldThread();
  }
}

#endif
