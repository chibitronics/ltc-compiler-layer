#include <stdint.h>

#include "Arduino.h"
#include "kl02.h"
#include "memio.h"

void early_init(void)
{
  /* Reset the CUP timer, to prevent a reset */
  writel(0x55, SIM_SRVCOP);
  writel(0xaa, SIM_SRVCOP);

  /* Ungate SPI0, UART0, CMP, and both I2C blocks */
  writel(readl(SIM_SCGC4) | (1 << 22) | (1 << 19) | (1 << 10) | (1 << 7) | (1 << 6), SIM_SCGC4);

  /* Set SIM_SCGC5 to enable PORTA, PORTB, and LPTMR */
  writel(readl(SIM_SCGC5) | (1 << 10) | (1 << 9) | (1 << 0), SIM_SCGC5);

  /* Ungate ADC, TPMs, and FTF */
  writel(readl(SIM_SCGC6) | (1 << 27) | (1 << 25) | (1 << 24) | (1 << 0), SIM_SCGC6);

  /* Set MCGIRCLK as the source for UART0 and TMPSRC */
  writel((3 << 26) | (3 << 24), SIM_SOPT2);

  return;
}

void setup(void) {
  pinMode(32 + 6, OUTPUT);
  pinMode(32 + 7, OUTPUT);
  pinMode(32 + 10, OUTPUT);
}

void loop(void) {
  static int loop = 0;

  loop++;

  digitalWrite(32 + 10, loop & 1);
  digitalWrite(32 + 7, loop & 2);
  digitalWrite(32 + 6, loop & 4);

  delay(500);
}

int main(void)
{
  double a = 2;
  double b = 3.0;
  setup();
  while (1) {
    b += a;
    loop();
  }
}
