#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "memio.h"

#include "wave-table.h"

// Define this to do some more math when generating a sample.
#define USE_INTERPOLATION

static int32_t next_sample;
static volatile uint8_t sample_queued;

// phase accumulator; this holds the current position in the sine wave of our sample
// if you want to generate more than one note, you'll need a phase accumulator for
// each note

// the desired harmonics - normalised so that the proportion required is scaled from
// 0 = minimum to 32767 = maximum
//
// Note that for perfect verisimiltude it would be necessary also to define the phase
// relationships between the harmonics - but it turns out that the human ear is not
// terribly good at discriminating phase within a waveform, which is why people put
// up with mpeg-type compressed audio.

// definitions

#define SAMPLERATE 14000 /* Measured period of i2c0_stable_timer */

int irq_counter = 0;
int main_loop_counter = 0;
int idle_loop_counter = 0;
static int pwm0_stable_timer(void)
{
  /* Reset the timer IRQ, to allow us to fire again next time */
  writel(TPM0_STATUS_CH1F, TPM0_STATUS);

  irq_counter++;

  static int loops = 0;
  if (loops++ >= 12) {
    loops = 0;
    writel(((next_sample + 127) & 0xff) + 2, TPM0_C1V);
    sample_queued = 0;
  }

//  writel((1 << 11), FGPIOB_PTOR);

  return 0;
}

static void prepare_pwm()
{
  pinMode(0, OUTPUT);
  analogWrite(0, 63);

  writel(0, TPM0_SC); // Disable TPM0

  // Configure the TPM to use the MCGFLLCLK (~32 MHz?)
  writel(readl(SIM_SOPT2) | (1 << 24), SIM_SOPT2);
  // We've picked pin 0, which is on TPM0_CH1
  writel(257, TPM0_MOD);
  writel(100, TPM0_C1V);
  writel(TPM0_C1SC_CHIE | TPM0_C1SC_MSB | TPM0_C1SC_ELSB, TPM0_C1SC);
  writel(TPM0_SC_TOF | TPM0_SC_CMOD(1) | TPM0_SC_PS(0), TPM0_SC); // Enable TPM0


  /* Enable the IRQ in the system-wide interrupt table */
  attachFastInterrupt(PWM0_IRQ, pwm0_stable_timer);
}

void setup(void)
{
  pinMode(1, OUTPUT); // Used for debugging
  prepare_pwm();
  enableInterrupt(PWM0_IRQ);
}

#define PHASEACC_MAX 256L

uint32_t position;
uint32_t distance;
uint32_t v1, v2, v1_weight, v2_weight;

int32_t get_sample(uint32_t *phaseacc, uint32_t freq)
{

  uint32_t period;
  int32_t output;

  // calculate the phase accumulator distance
  // we divide the frequency by the sample rate to give us how much of a cycle occurs
  // between successive samples... assuming a frequency range of 20Hz-20kHz this would
  // be on the order of 0.0004 to 0.4, so we multiply it to give us a meaningful range

  period = (freq * PHASEACC_MAX) / SAMPLERATE;

  // add this to the phase accumulator
  *phaseacc += period;

  // wrap the phase accumulator around
  *phaseacc &= (PHASEACC_MAX - 1);

  // and now get the sine output values for each of the allowed harmonics
  // to be elegant (and to improve the sound quality) we should really
  // interpolate between the current table entry and the next one by an amount
  // proportional to the position between the two entries, but this is just
  // an example so we won't bother for now
  position = (*phaseacc * SINE_TABLE_SIZE) / PHASEACC_MAX;

  // Interpolation happens because there are "gaps" that are between the phase
  // accumulator and the table.
#ifdef USE_INTERPOLATION
  // This is how far off we are.  I.e. the error.
  distance = *phaseacc - ((position * PHASEACC_MAX) / SINE_TABLE_SIZE);

  // And this is how many "Gaps" there are total between two entries in the table
  static uint32_t gap = PHASEACC_MAX / SINE_TABLE_SIZE;

  v1 = sine_table[position];
  position++;
  if (position >= SINE_TABLE_SIZE)
    position -= SINE_TABLE_SIZE;
  v2 = sine_table[position];

  v1_weight = gap - distance;
  v2_weight = gap - v1_weight;

  output = ((v1 * v1_weight) + (v2 * v2_weight)) / gap;
#else
  output = sine_table[position];
#endif

  return output;
}

void loop(void)
{
  static uint32_t phaseacc_1 = 0;
  static uint32_t phaseacc_2 = 0;
  static uint32_t freqs[] = {440, 495, 557, 660, 742};
  static uint32_t freq_num = 0;
  static uint32_t other_freq_num;

  main_loop_counter++;
  if (!(main_loop_counter & 0xffff)) {
    freq_num++;
    if (freq_num > 4)
      freq_num = 0;
  }

  if (!(idle_loop_counter & 0xffff)) {
    other_freq_num++;
    if (other_freq_num > 4)
      other_freq_num = 0;
  }

  if (sample_queued) {
    idle_loop_counter++;
    return;
  }

  next_sample = 0;
  //next_sample += get_sample(&phaseacc_1, 440);
  next_sample += get_sample(&phaseacc_1, freqs[freq_num]) / 2;
  next_sample += get_sample(&phaseacc_2, freqs[other_freq_num]) / 2;
  sample_queued = 1;
}
