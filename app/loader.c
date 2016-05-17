#include "app.h"
#include "Arduino.h"

/* Variables provided by the linker */
extern uint32_t _textdata;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _bss_start;
extern uint32_t _bss_end;
extern uint32_t __init_array_start;
extern uint32_t __init_array_end;
extern uint32_t __heap_base__;
extern uint32_t __heap_end__;

__attribute__((naked, noreturn))
void Esplanade_Main(void) {

  setup();

  while (1)
    loop();
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
  .version          = APP_VERSION,
  .const_start      = &__init_array_start,
  .const_end        = &__init_array_end,
  .heap_start       = &__heap_base__,
  .heap_end         = &__heap_end__,
};
