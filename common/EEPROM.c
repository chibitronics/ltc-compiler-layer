#include <string.h>

#include "Arduino.h"
#ifndef E2END
#define E2END 511
#endif

typedef struct {
  uint32_t data[5];
} virtual_timer_t;
typedef uint32_t systime_t;
typedef void (*vtfunc_t)(void *p);

virtual_timer_t my_timer;

extern void setTimer(virtual_timer_t *vtp, systime_t delay,
                     vtfunc_t vtfunc, void *par);
extern void unlockSystemFromISR(void);
extern void lockSystemFromISR(void);
extern int8_t flashErase(uint32_t sectorOffset, uint16_t sectorCount);
extern int8_t flashWrite(uint8_t *src, const uint8_t *dst, uint32_t count);

__attribute__((section(".flasheeprom")))
static const uint8_t *flash_area;

static uint8_t flash_backing[E2END];

static enum {
  STATE_EMPTY,
  STATE_DIRTY,
  STATE_CLEAN,
} flash_state;

void eeprom_flush(void *arg) {
  (void)arg;

  lockSystemFromISR();
  flashErase(((uint32_t)flash_area) / 1024, 1);
  flashWrite((uint8_t *)flash_backing, flash_area, sizeof(flash_backing));
  flash_state = STATE_CLEAN;
  unlockSystemFromISR();
}

uint8_t eeprom_read_byte(uint8_t *index) {
  return flash_area[(uint32_t)index];
}

uint8_t eeprom_write_byte(uint8_t *index, uint8_t val) {
  if (((uint32_t)index) > E2END)
    return 1;

  if (flash_state == STATE_EMPTY)
    memcpy(flash_backing, (const void *)flash_area, E2END);

  flash_backing[(uint32_t)index] = val;
  flash_state = STATE_DIRTY;

  setTimer(&my_timer, 1000, eeprom_flush, NULL);

  return 0;
}