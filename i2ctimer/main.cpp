#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "memio.h"

static int counter = 0;
static int i2c0_stable_timer(void) {

  /* Ordinarily, we'd need to write a "1" to I2C0_S to acknowledge
   * the IRQ.  That would allow us to re-queue another message.
   * However, since there isn't actually anything attached, we
   * get here because of a NAK.
   *
   * To keep the timer going, tear down I2C and restart the session.
   */

  /* Write C1 to enable TX */
  writeb((1 << 7) | (1 << 6) | (1 << 4), I2C0_C1);

  /* Write C1 to enable MST */
  writeb((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4), I2C0_C1);

  /* Write out another byte, to start another transfer */
  writeb(0x55, I2C0_D);

  /* Toggle pin 4 (PTA12) */
  if ((counter & 0xffff) == 0)
    writel((1 << 12), FGPIOA_PTOR);
  counter++;

  return 0;
}

void setup(void) {

  /* Pin A2 is unshared, so use it for output to avoid confusion */
  pinMode(A2, OUTPUT);

  /* Mux the unattached pin as SDA (ALT2) (with slow slew rate) */
  writel((2 << 8) | (1 << 2), PORTB_PCR4);

  /* Ungate I2C0 */
  writel(readl(SIM_SCGC4) | (1 << 6), SIM_SCGC4);

  /* Write some frequency */
  writeb(0x18, I2C0_F);
  
  /* Write C1 to enable module and interrupts */
  writeb((1 << 7) | (1 << 6), I2C0_C1);

  /* Write C1 to enable TX */
  writeb((1 << 7) | (1 << 6) | (1 << 4), I2C0_C1);

  /* Write C1 to enable MST */
  writeb((1 << 7) | (1 << 6) | (1 << 5) | (1 << 4), I2C0_C1);

  /* Enable the IRQ in the system-wide interrupt table */
  attachFastInterrupt(I2C0_IRQ, i2c0_stable_timer);
  enableInterrupt(I2C0_IRQ);

  /* Kick off the transfer by writing a data byte */
  writeb(0x55, I2C0_D);
}

void loop(void) {
  ;
}
