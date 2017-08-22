#include <stdint.h>
#include "Arduino.h"
#include "kl02.h"

extern void ledUpdate(uint32_t num_leds, void *pixels, uint32_t mask,
                      uint32_t set_addr, uint32_t clr_addr);
void ledShow(uint32_t pin, void *pixels, uint32_t num_leds) {

  uint32_t port;
  uint8_t pad;

  pin = canonicalizePin(pin);

  /* Only work with the RGB pin */
  if (pin != LED_BUILTIN_RGB)
    return;

  if (pinToPort(pin, &port, &pad) < 0)
    return;

  if (port == GPIOA_BASE)
    port = FGPIOA_BASE;
  else if (port == GPIOB_BASE)
    port = FGPIOB_BASE;
  else
    return;

  __disable_irq();
  ledUpdate(num_leds, pixels, 1 << pad,
      port + GPIO_PSOR,
      port + GPIO_PCOR);
  __enable_irq();
}
