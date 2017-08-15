#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "memio.h"

#include "wave-table.h"

// Enable interpolation to make the output smoother.
// Set to 0 to disable interpolation.
// Note that some instruments don't support interpolation.
#define INTERPOLATION_ENABLED 1

// Sets the maximum value of the phase accumulator, which is
// used to skip through the sample array.
#define PHASEACC_MAX 16384L

// The system is running off of a 32.768 kHz crystal going through
// a 1464x FLL multiplier, giving a system frequency of
// 47.972352 MHz.
// We set the PWM counter to 258, which gives a PWM period of 185.939 kHz.
// Happily, 185.939 is evenly divisible by 13, giving us an actual sample
// rate of 14303 kHz, assuming we delay 13 times.
#define PWM_DELAY_LOOPS 13
#define SAMPLE_RATE 14303

// The number of ticks that the sound system has gone through.
// Overflows after about three days, at 14 kHz.
volatile uint32_t global_tick_counter;

// The next sample to be played, nominally between -128 and 127
static int32_t next_sample;

// Nonzero if a sample has been queued, zero if the sample buffer is empty.
static volatile uint8_t sample_queued;

// An ltc voice 
struct ltc_voice {
  // The current note's frequency.
  uint32_t frequency;

  // A pointer to the currently-selected instrument.
  const struct ltc_instrument *instrument;

  uint32_t attack_time;
  uint32_t attack_level;
  uint32_t delay_time;
  uint32_t sustain_level;
  uint32_t release_time;

  // Keeps track of the phase in the instrument at the
  // given frequency.
  uint32_t phase_accumulator;

  // The time when the note timer started.
  uint32_t note_timer;

  // What time the note was released
  uint32_t release_timer;
};

static struct ltc_voice voice[2];

static int pwm0_stable_timer(void)
{
  static int loops = 0;

  /* Reset the timer IRQ, to allow us to fire again next time */
  writel(TPM0_STATUS_CH1F | TPM0_STATUS_CH0F | TPM0_STATUS_TOF, TPM0_STATUS);

  if (loops++ > PWM_DELAY_LOOPS) {
    int32_t scaled_sample = next_sample + 130;
    if (scaled_sample > 255)
      scaled_sample = 255;
    if (scaled_sample <= 0)
      scaled_sample = 2;
    writel(scaled_sample, TPM0_C1V);
    writel(scaled_sample, TPM0_C0V);

    loops = 0;
    sample_queued = 0;
    global_tick_counter++;
  }

  return 0;
}

static void prepare_pwm()
{
  // Write dummy values out, to configure PWM mux
  pinMode(0, OUTPUT);
  analogWrite(0, 63);
  pinMode(1, OUTPUT);
  analogWrite(1, 63);

  // Disable TPM0, allowing us to configure it
  writel(0, TPM0_SC);

  // Also disable both channels, which are running from the
  // calls to analogWrite() above
  writel(0, TPM0_C0SC);
  writel(0, TPM0_C1SC);

  // Configure the TPM to use the MCGFLLCLK (~32 MHz?)
  writel(readl(SIM_SOPT2) | (1 << 24), SIM_SOPT2);

  // We've picked pin 0, which is on TPM0_CH1
  writel(257, TPM0_MOD);
  writel(0, TPM0_CNT);

  writel(TPM0_C0SC_MSB | TPM0_C0SC_ELSB, TPM0_C0SC);
  writel(TPM0_C1SC_MSB | TPM0_C1SC_ELSA, TPM0_C1SC);

  writel(100, TPM0_C1V);
  writel(100, TPM0_C0V);
  writel(TPM0_CONF_TRGSEL(8) | TPM0_CONF_DBGMODE, TPM0_CONF);
  writel(TPM0_SC_TOF | TPM0_SC_TOIE | TPM0_SC_CMOD(1) | TPM0_SC_PS(0), TPM0_SC); // Enable TPM0

  /* Enable the IRQ in the system-wide interrupt table */
  attachFastInterrupt(PWM0_IRQ, pwm0_stable_timer);
}

static void setInstrument(struct ltc_voice *voice,
                          const struct ltc_instrument *instrument,
                          uint32_t attack_time, uint32_t attack_level,
                          uint32_t delay_time,
                          uint32_t sustain_level,
                          uint32_t release_time)
{
  voice->instrument = instrument;
  voice->attack_time = (attack_time * SAMPLE_RATE) / 1000;
  voice->attack_level = attack_level;
  voice->delay_time = (delay_time * SAMPLE_RATE) / 1000 + voice->attack_time;
  voice->sustain_level = sustain_level;
  voice->release_time = (release_time * SAMPLE_RATE) / 1000;
}

void setup(void)
{
  setInstrument(&voice[0],
    &sawtooth_instrument,
    450, 50,
    250,
    40,
    50);

  setInstrument(&voice[1],
    &sine_instrument,
    150, 50,
    100,
    40,
    50);
  
  prepare_pwm();
  enableInterrupt(PWM0_IRQ);
}

// attack_time = 300
// delay_time = 500
// attack_level = 100
// sustain_level = 70
// note_timer = 300: pct = 100
// note_timer = 500: pct = 70
// note_timer = 400: pct = 85
static int32_t processADSR(struct ltc_voice *voice, int32_t output)
{
  if (voice->note_timer < voice->attack_time) {
    /* Attack phase */

    /* Determine what percentage we'll adjust the note to */
    int32_t pct = voice->note_timer * voice->attack_level / voice->attack_time;

    /* Scale the note volume to the calcualted percentage */
    output = output * pct / 100;
  }
  else if (voice->note_timer < voice->delay_time) {
    /* Delay phase */

    /* Determine what percentage we'll adjust the note to */
    int32_t pct = voice->sustain_level + (voice->delay_time - voice->note_timer) * (voice->attack_level - voice->sustain_level) / (voice->delay_time - voice->attack_time);

    /* Scale the note volume to the calcualted percentage */
    output = output * pct / 100;
  }
  else if (voice->release_timer != 0) {
    /* Release phase */

    /* The note has expired */
    if ((voice->note_timer - voice->release_timer) > voice->release_time)
      return 0;

    /* Determine what percentage we'll adjust the note to */
    int32_t pct = (voice->note_timer - voice->release_timer) * voice->sustain_level / voice->release_time;

    /* Scale the note volume to the calcualted percentage */
    output = output * pct / 100;
  }
  else {
    /* Sustain phase */
    output = output * voice->sustain_level / 100;
  }
  return output;
}

int32_t get_sample(struct ltc_voice *voice)
{
  uint32_t period;
  int32_t output;
  int32_t v1, v2, v1_weight, v2_weight;

  // calculate the phase accumulator distance
  // we divide the frequency by the sample rate to give us how much of a cycle occurs
  // between successive samples... assuming a frequency range of 20Hz-20kHz this would
  // be on the order of 0.0004 to 0.4, so we multiply it to give us a meaningful range

  period = (voice->frequency * PHASEACC_MAX) / SAMPLE_RATE;

  // add this to the phase accumulator
  voice->phase_accumulator += period;

  // wrap the phase accumulator around
  voice->phase_accumulator &= (PHASEACC_MAX - 1);

  // and now get the sine output values for each of the allowed harmonics
  // to be elegant (and to improve the sound quality) we should really
  // interpolate between the current table entry and the next one by an amount
  // proportional to the position between the two entries, but this is just
  // an example so we won't bother for now
  uint32_t position = (voice->phase_accumulator * voice->instrument->length) / PHASEACC_MAX;

  // Interpolation happens because there are "gaps" that are between the phase
  // accumulator and the table.
  if (INTERPOLATION_ENABLED && (voice->instrument->flags & INSTRUMENT_CAN_INTERPOLATE))
  {
    // This is how far off we are.  I.e. the error.
    int32_t distance = voice->phase_accumulator - ((position * PHASEACC_MAX) / voice->instrument->length);

    // And this is how many "Gaps" there are total between two entries in the table
    const int32_t gap = PHASEACC_MAX / voice->instrument->length;

    v1 = voice->instrument->samples[position];
    position++;
    if (position >= voice->instrument->length)
      position -= voice->instrument->length;
    v2 = voice->instrument->samples[position];

    v1_weight = gap - distance;
    v2_weight = gap - v1_weight;

    output = ((v1 * v1_weight) + (v2 * v2_weight)) / gap;
  } else {
    output = voice->instrument->samples[position];
  }

  output = processADSR(voice, output);

  voice->note_timer++;
  return output;
}

static void noteOn(struct ltc_voice *voice, uint32_t freq) {
  voice->frequency = freq;
  voice->note_timer = 0;
  voice->release_timer = 0;
}

static void noteOff(struct ltc_voice *voice) {
  voice->release_timer = voice->note_timer;
}

void loop(void)
{
  static uint16_t frequencies[] = {440, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783, 830, 880};

  // If a sample is still in the buffer, don't do anything.
  if (sample_queued) {
    return;
  }

  static int note_state;
  static int current_note;
  if ((global_tick_counter & 0x1fff) == 0) {
    if (note_state) {
      noteOff(&voice[0]);
      noteOff(&voice[1]);
      digitalWrite(2, 0);
      current_note++;
    }
    else {
      noteOn(&voice[0], frequencies[current_note % 12]);
      noteOn(&voice[1], frequencies[(current_note + 5) % 12]);
      digitalWrite(2, 1);
      current_note++;
    }
    note_state = !note_state;
  }

  next_sample = 0;
  next_sample += get_sample(&voice[0]);
  next_sample += get_sample(&voice[1]);
  sample_queued = 1;
}
