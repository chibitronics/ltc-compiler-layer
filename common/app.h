#ifndef __APP_H__
#define __APP_H__

#include <stdint.h>

struct app_header {
  uint32_t *data_load_start; /* Start of data in ROM */
  uint32_t *data_start;      /* Start of data load address in RAM */
  uint32_t *data_end;        /* End of data load address in RAM */
  uint32_t *bss_start;       /* Start of BSS in RAM */
  uint32_t *bss_end;         /* End of BSS in RAM */
  void (*entry)(void);      /* Address to jump to */
  uint32_t reserved1;
  uint32_t reserved2;
};

#endif
