#include <stdint.h>

#include "memio.h"

void early_init(void)
{
  /* Reset the CUP timer, to prevent a reset */
  writel(0x55, 0x40048104);
  writel(0xaa, 0x40048104);

  /* GPIOs connected to: PTB6 (R), PTB7 (G), PTB10 (B) */
  /* Set SIM_SCGC5 to enable PORTA, PORTB, and LPTMR */
  writel(0x00000713, 0x40048038);

  writel(0x00000100, 0x4004a018); // Set PTB6 to GPIO
  writel(0x00000100, 0x4004a01c); // Set PTB7 to GPIO
  writel(0x00000100, 0x4004a028); // Set PTB10 to GPIO

  /* Set PTB6, PTB7, and PTB10 to output */
  writel((1 << 10) | (1 << 7) | (1 << 6), 0x400ff054);

  return;
}

int main(void)
{
  int loop = 0;
  int i;

  while (1) {
    loop++;
    if (loop & 1)
      writel(1 << 10, 0x400ff04c); // toggle PTB10
    if (loop & 2)
      writel(1 << 7, 0x400ff04c); // toggle PTB7
    if (loop & 4)
      writel(1 << 6, 0x400ff04c); // toggle PTB6

    for (i = 0; i < 1000000; i++)
      asm("nop");
  }
}
