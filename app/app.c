#include "app.h"

extern uint32_t _textdata;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss_start;
extern uint32_t _bss_end;

int nonzero = 5;
int zero = 0;

void Esplanade_Main(void) {
  nonzero++;
  zero++;

  while (nonzero != zero)
    zero++;

  return;
}

__attribute__ ((used, section(".progheader")))
struct app_header app_header = {
  .data_load_start  = &_textdata,
  .data_start       = &_data,
  .data_end         = &_edata,
  .bss_start        = &_bss_start,
  .bss_end          = &_bss_end,
  .entry            = Esplanade_Main,
  .magic            = APP_MAGIC,
};
