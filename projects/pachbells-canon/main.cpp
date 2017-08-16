#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "memio.h"

#include "wave-table.h"

uint16_t timesteps[] = { 0, 512, 512, 1024, 1024, 1536, 1536, 2048, 2048, 2560, 2560, 3072, 3072, 3584, 3584, 4096, 4096, 4608, 4608, 5120, 5120, 5632, 5632, 6144, 6144, 6656, 6656, 7168, 7168, 7680, 7680, 8192, 8192, 8704, 8704, 9216, 9216, 9728, 9728, 10240, 10240, 10752, 10752, 11264, 11264, 11776, 11776, 12288, 12288, 12416, 12416, 12544, 12544, 12672, 12672, 12800, 12800, 13056, 13056, 13312, 13312, 13568, 13568, 13824, 13824, 14208, 14208, 14336, 14336, 14464, 14464, 14592, 14592, 14720, 14720, 14848, 14848, 14976, 14976, 15104, 15104, 15232, 15232, 15360, 15360, 15488, 15488, 15616, 15616, 15744, 15744, 15872, 15872, 16128, 16128, 16384, 16384, 16512, 16512, 16640, 16640, 16768, 16768, 16896, 16896, 17152, 17152, 17408, 17408, 17664, 17664, 17920, 17920, 18304, 18304, 18432, 18432, 18560, 18560, 18688, 18688, 18816, 18816, 18944, 18944, 19072, 19072, 19200, 19200, 19328, 19328, 19456, 19456, 19584, 19584, 19712, 19712, 19840, 19840, 19968, 19968, 20224, 20224, 20480, 20480, 20608, 20608, 20672, 20672, 20736, 20736, 20864, 20864, 20928, 20928, 20992, 20992, 21056, 21056, 21120, 21120, 21184, 21184, 21248, 21248, 21312, 21312, 21376, 21376, 21440, 21440, 21504, 21504, 21632, 21632, 21696, 21696, 21760, 21760, 21888, 21888, 21952, 21952, 22016, 22016, 22080, 22080, 22144, 22144, 22208, 22208, 22272, 22272, 22336, 22336, 22400, 22400, 22464, 22464, 22528, 22528, 22656, 22656, 22720, 22720, 22784, 22784, 22912, 22912, 22976, 22976, 23040, 23040, 23104, 23104, 23168, 23168, 23232, 23232, 23296, 23296, 23360, 23360, 23424, 23424, 23488, 23488, 23552, 23552, 23680, 23680, 23744, 23744, 23808, 23808, 23936, 23936, 24000, 24000, 24064, 24064, 24128, 24128, 24192, 24192, 24256, 24256, 24320, 24320, 24384, 24384, 24448, 24448, 24512, 24512, 24576, 24576, 24704, 24704, 24768, 24768, 24832, 24832, 24960, 24960, 25024, 25024, 25088, 25088, 25152, 25152, 25216, 25216, 25280, 25280, 25344, 25344, 25408, 25408, 25472, 25472, 25536, 25536, 25600, 25600, 25728, 25728, 25792, 25792, 25856, 25856, 25984, 25984, 26048, 26048, 26112, 26112, 26176, 26176, 26240, 26240, 26304, 26304, 26368, 26368, 26432, 26432, 26496, 26496, 26560, 26560, 26624, 26624, 26752, 26752, 26816, 26816, 26880, 26880, 27008, 27008, 27072, 27072, 27136, 27136, 27200, 27200, 27264, 27264, 27328, 27328, 27392, 27392, 27456, 27456, 27520, 27520, 27584, 27584, 27648, 27648, 27776, 27776, 27840, 27840, 27904, 27904, 28032, 28032, 28096, 28096, 28160, 28160, 28224, 28224, 28288, 28288, 28352, 28352, 28416, 28416, 28480, 28480, 28544, 28544, 28608, 28608, 28672, 28672, 28800, 28800, 28864, 28864, 28928, 28928, 29056, 29056, 29184, 29184, 29312, 29312, 29376, 29376, 29440, 29440, 29568, 29568, 29696, 29696, 29824, 29824, 29888, 29888, 29952, 29952, 30080, 30080, 30208, 30208, 30336, 30336, 30400, 30400, 30464, 30464, 30592, 30592, 30720, 30720, 30848, 30848, 30912, 30912, 30976, 30976, 31104, 31104, 31232, 31232, 31360, 31360, 31424, 31424, 31488, 31488, 31616, 31616, 31744, 31744, 31872, 31872, 31936, 31936, 32000, 32000, 32128, 32128, 32256, 32256, 32384, 32384, 32448, 32448, 32512, 32512, 32640, 32640, 32768, 32768, 33152, 33152, 33280, 33280, 33408, 33408, 33536, 33536, 33664, 33664, 33792, 33792, 34176, 34176, 34304, 34304, 34432, 34432, 34560, 34560, 34688, 34688, 34816, 34816, 35328, 35328, 35840, 35840, 35968, 35968, 36096, 36096, 36224, 36224, 36352, 36352, 36864, 36864, 37120, 37120, 37248, 37248, 37376, 37376, 37504, 37504, 37632, 37632, 37760, 37760, 37888, 37888, 38272, 38272, 38400, 38400, 38528, 38528, 38656, 38656, 38784, 38784, 38912, 38912, 39040, 39040, 39168, 39168, 39296, 39296, 39424, 39424, 39936, 39936, 40064, 40064, 40192, 40192, 40320, 40320, 40448, 40448, 40832, 40832, 40960, 40960, 41216, 41216, 41344, 41344, 41472, 41472, 41600, 41600, 41728, 41728, 41856, 41856, 41984, 41984, 42368, 42368, 42496, 42496, 42624, 42624, 42752, 42752, 42880, 42880, 43008, 43008, 43136, 43136, 43264, 43264, 43392, 43392, 43520, 43520, 44032, 44032, 44160, 44160, 44288, 44288, 44416, 44416, 44544, 44544, 44928, 44928, 45056, 45056, 45312, 45312, 45440, 45440, 45568, 45568, 45696, 45696, 45824, 45824, 45952, 45952, 46080, 46080, 46336, 46336, 46848, 46848, 47104, 47104, 47616, 47616, 48128, 48128, 48640, 48640, 49152, 49152, 49664, 49664, 50176, 50176, 50688, 50688, 51200, 51200, 51712, 51712, 52224, 52224, 52736, 52736, 53248, 53248, 53760, 53760, 54272, 54272, 54784, 54784, 55296, 55296, 55808, 55808, 56320, 56320, 56832, 56832, 57344, 57344, 58368};
uint8_t notes[] = {  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  83,  83,  85,  85,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  83,  83,  85,  85,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  83,  83,  85,  85,  86,  86,  85,  85,  86,  86,  78,  78,  81,  81,  85,  85,  86,  86,  90,  90,  93,  93,  95,  95,  91,  91,  90,  90,  88,  88,  91,  91,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  79,  79,  78,  78,  76,  76,  85,  85,  86,  86,  85,  85,  86,  86,  78,  78,  81,  81,  85,  85,  86,  86,  90,  90,  93,  93,  95,  95,  91,  91,  90,  90,  88,  88,  91,  91,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  79,  79,  78,  78,  76,  76,  85,  85,  86,  86,  90,  90,  91,  91,  93,  93,  90,  90,  91,  91,  93,  93,  81,  81,  83,  83,  85,  85,  86,  86,  88,  88,  90,  90,  91,  91,  90,  90,  86,  86,  88,  88,  90,  90,  78,  78,  79,  79,  81,  81,  83,  83,  81,  81,  79,  79,  81,  81,  78,  78,  79,  79,  81,  81,  79,  79,  83,  83,  81,  81,  79,  79,  78,  78,  76,  76,  78,  78,  76,  76,  74,  74,  76,  76,  78,  78,  79,  79,  81,  81,  83,  83,  79,  79,  83,  83,  81,  81,  83,  83,  85,  85,  86,  86,  85,  85,  83,  83,  85,  85,  86,  86,  88,  88,  90,  90,  91,  91,  88,  88,  86,  86,  90,  90,  91,  91,  93,  93,  90,  90,  91,  91,  93,  93,  81,  81,  83,  83,  85,  85,  86,  86,  88,  88,  90,  90,  91,  91,  90,  90,  86,  86,  88,  88,  90,  90,  78,  78,  79,  79,  81,  81,  83,  83,  81,  81,  79,  79,  81,  81,  78,  78,  79,  79,  81,  81,  79,  79,  83,  83,  81,  81,  79,  79,  78,  78,  76,  76,  78,  78,  76,  76,  74,  74,  76,  76,  78,  78,  79,  79,  81,  81,  83,  83,  79,  79,  83,  83,  81,  81,  83,  83,  85,  85,  86,  86,  85,  85,  83,  83,  85,  85,  86,  86,  88,  88,  90,  90,  91,  91,  88,  88,  90,  90,  86,  86,  85,  85,  86,  86,  78,  78,  81,  81,  85,  85,  86,  86,  88,  88,  85,  85,  83,  83,  86,  86,  88,  88,  90,  90,  86,  86,  90,  90,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  83,  83,  81,  81,  83,  83,  85,  85,  86,  86,  90,  90,  88,  88,  86,  86,  93,  93,  91,  91,  86,  86,  85,  85,  83,  83,  83,  83,  81,  81,  81,  81,  79,  79,  78,  78,  76,  76,  86,  86,  90,  90,  90,  90,  91,  91,  90,  90,  88,  88,  86,  86,  86,  86,  86,  86,  88,  88,  86,  86,  85,  85,  83,  83,  86,  86,  86,  86,  84,  84,  83,  83,  84,  84,  81,  81,  86,  86,  90,  90,  93,  93,  93,  93,  95,  95,  93,  93,  91,  91,  90,  90,  90,  90,  90,  90,  91,  91,  90,  90,  88,  88,  86,  86,  84,  84,  83,  83,  84,  84,  86,  86,  86,  86,  84,  84,  83,  83,  84,  84,  85,  85,  85,  85,  86,  86,  90,  90,  93,  93,  93,  93,  95,  95,  93,  93,  91,  91,  90,  90,  90,  90,  90,  90,  91,  91,  90,  90,  88,  88,  86,  86,  84,  84,  83,  83,  84,  84,  86,  86,  86,  86,  84,  84,  83,  83,  84,  84,  85,  85,  85,  85,  86,  86,  90,  90,  93,  93,  93,  93,  95,  95,  93,  93,  91,  91,  90,  90,  98,  98,  97,  97,  95,  95,  93,  93,  95,  95,  97,  97,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  83,  83,  85,  85,  90,  90,  88,  88,  86,  86,  85,  85,  83,  83,  81,  81,  83,  83,  85,  85,  86,  86};
uint8_t velocities[] = { 58,  0,  53,  0,  52,  0,  53,  0,  52,  0,  58,  0,  63,  0,  67,  0,  98,  0,  79,  0,  81,  0,  78,  0,  77,  0,  77,  0,  89,  0,  95,  0,  95,  0,  78,  0,  76,  0,  79,  0,  80,  0,  81,  0,  89,  0,  90,  0,  89,  0,  80,  0,  87,  0,  73,  0,  88,  0,  97,  0,  87,  0,  91,  0,  86,  0,  92,  0,  76,  0,  81,  0,  74,  0,  87,  0,  81,  0,  84,  0,  79,  0,  77,  0,  81,  0,  81,  0,  84,  0,  79,  0,  80,  0,  101,  0,  92,  0,  84,  0,  87,  0,  72,  0,  86,  0,  94,  0,  88,  0,  94,  0,  98,  0,  91,  0,  75,  0,  75,  0,  75,  0,  93,  0,  77,  0,  80,  0,  77,  0,  79,  0,  76,  0,  80,  0,  76,  0,  78,  0,  76,  0,  102,  0,  89,  0,  95,  0,  90,  0,  89,  0,  78,  0,  88,  0,  90,  0,  63,  0,  89,  0,  84,  0,  91,  0,  86,  0,  89,  0,  87,  0,  80,  0,  77,  0,  90,  0,  89,  0,  63,  0,  85,  0,  89,  0,  91,  0,  77,  0,  80,  0,  92,  0,  75,  0,  91,  0,  93,  0,  76,  0,  90,  0,  79,  0,  76,  0,  77,  0,  81,  0,  96,  0,  83,  0,  77,  0,  93,  0,  91,  0,  89,  0,  88,  0,  92,  0,  71,  0,  93,  0,  78,  0,  88,  0,  88,  0,  92,  0,  80,  0,  78,  0,  89,  0,  92,  0,  93,  0,  88,  0,  90,  0,  71,  0,  78,  0,  94,  0,  86,  0,  87,  0,  81,  0,  87,  0,  89,  0,  63,  0,  85,  0,  90,  0,  89,  0,  91,  0,  87,  0,  93,  0,  82,  0,  76,  0,  87,  0,  92,  0,  62,  0,  85,  0,  89,  0,  89,  0,  73,  0,  81,  0,  89,  0,  78,  0,  92,  0,  89,  0,  78,  0,  88,  0,  80,  0,  77,  0,  78,  0,  81,  0,  88,  0,  77,  0,  77,  0,  93,  0,  86,  0,  87,  0,  90,  0,  88,  0,  74,  0,  91,  0,  79,  0,  90,  0,  91,  0,  88,  0,  78,  0,  78,  0,  88,  0,  91,  0,  87,  0,  91,  0,  87,  0,  77,  0,  89,  0,  75,  0,  83,  0,  88,  0,  69,  0,  91,  0,  90,  0,  87,  0,  89,  0,  78,  0,  80,  0,  91,  0,  91,  0,  94,  0,  75,  0,  92,  0,  84,  0,  81,  0,  78,  0,  78,  0,  81,  0,  84,  0,  83,  0,  92,  0,  91,  0,  90,  0,  94,  0,  79,  0,  78,  0,  94,  0,  82,  0,  70,  0,  81,  0,  78,  0,  87,  0,  79,  0,  85,  0,  77,  0,  79,  0,  79,  0,  100,  0,  100,  0,  84,  0,  82,  0,  77,  0,  77,  0,  77,  0,  81,  0,  88,  0,  88,  0,  74,  0,  76,  0,  79,  0,  94,  0,  86,  0,  74,  0,  80,  0,  87,  0,  71,  0,  94,  0,  98,  0,  89,  0,  90,  0,  86,  0,  78,  0,  75,  0,  80,  0,  81,  0,  85,  0,  86,  0,  78,  0,  78,  0,  80,  0,  80,  0,  75,  0,  87,  0,  88,  0,  86,  0,  75,  0,  80,  0,  84,  0,  86,  0,  82,  0,  89,  0,  90,  0,  94,  0,  86,  0,  93,  0,  82,  0,  81,  0,  75,  0,  82,  0,  83,  0,  93,  0,  81,  0,  79,  0,  79,  0,  81,  0,  77,  0,  90,  0,  88,  0,  88,  0,  79,  0,  80,  0,  89,  0,  90,  0,  80,  0,  89,  0,  93,  0,  89,  0,  86,  0,  88,  0,  73,  0,  76,  0,  81,  0,  102,  0,  82,  0,  77,  0,  76,  0,  95,  0,  87,  0,  70,  0,  77,  0,  75,  0,  80,  0,  81,  0,  78,  0,  89,  0,  87,  0,  95,  0,  80,  0,  82,  0,  82,  0,  83,  0,  77,  0,  83,  0,  94,  0,  84,  0 };

static const uint16_t midi_freq[] = {
  8, 8, 9, 9, 10, 10, 11, 12, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 23, 24, 25, 27, 29, 30, 32, 34, 36, 38, 41, 43, 46,
  48, 51, 55, 58, 61, 65, 69, 73, 77, 82, 87, 92, 97, 103, 110, 116, 123, 130, 138, 146, 155, 164, 174, 184, 195, 207, 
  220, 233, 246, 261, 277, 293, 311, 329, 349, 369, 391, 415, 440, 466, 493, 523, 554, 587, 622, 659, 698, 739, 783, 830,
  880, 932, 987, 1046, 1108, 1174, 1244, 1318, 1396, 1479, 1567, 1661, 1760, 1864, 1975, 2093, 2217, 2349, 2489, 2637,
  2793, 2959, 3135, 3322, 3520, 3729, 3951, 4186, 4434, 4698, 4978, 5274, 5587, 5919, 6271, 6644, 7040, 7458, 7902, 8372,
  8869, 9397, 9956, 10548, 11175, 11839
};

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
// We set the PWM counter to 256, which gives a PWM period of 187.392 kHz.
// Happily, 185.939 is evenly divisible by 24, giving us an actual sample
// rate of 7808 Hz, assuming we delay 24 times.
#define PWM_DELAY_LOOPS 24
#define SAMPLE_RATE 7808

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
    int32_t scaled_sample = next_sample + 129;
    if (scaled_sample > 255)
      scaled_sample = 255;
    if (scaled_sample < 1)
      scaled_sample = 1;
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
  writel(255, TPM0_MOD);
  writel(0, TPM0_CNT);

  writel(TPM0_C0SC_MSB | TPM0_C0SC_ELSB, TPM0_C0SC);
  writel(TPM0_C1SC_MSB | TPM0_C1SC_ELSA, TPM0_C1SC);

  writel(100, TPM0_C1V);
  writel(100, TPM0_C0V);
  writel(TPM0_CONF_TRGSEL(8), TPM0_CONF);
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
    &square_instrument,
    450, 50,
    250,
    20,
    50);

  setInstrument(&voice[1],
    &sine_instrument,
    150, 50,
    100,
    40,
    50);
  
  prepare_pwm();
  enableInterrupt(PWM0_IRQ);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

static int32_t processADSR(struct ltc_voice *voice, int32_t output)
{
  int32_t pct;
  if (voice->release_timer != 0) {
    /* Release phase */
    writel((1 << 7), FGPIOA_PSOR); // 5
    writel((1 << 0), FGPIOB_PCOR); // 4
    writel((1 << 13), FGPIOB_PCOR); // 3
    writel((1 << 12), FGPIOA_PCOR); // 2

    /* The note has expired */
    if ((voice->note_timer - voice->release_timer) > voice->release_time)
      return 0;

    /* Determine what percentage we'll adjust the note to */
    pct = (voice->note_timer - voice->release_timer) * voice->sustain_level / voice->release_time;
  }
  else if (voice->note_timer < voice->attack_time) {
    /* Attack phase */
    writel((1 << 7), FGPIOA_PCOR); // 5
    writel((1 << 0), FGPIOB_PCOR); // 4
    writel((1 << 13), FGPIOB_PCOR); // 3
    writel((1 << 12), FGPIOA_PSOR); // 2

    /* Determine what percentage we'll adjust the note to */
    pct = voice->note_timer * voice->attack_level / voice->attack_time;
  }
  else if (voice->note_timer < voice->delay_time) {
    /* Delay phase */
    writel((1 << 7), FGPIOA_PCOR); // 5
    writel((1 << 0), FGPIOB_PCOR); // 4
    writel((1 << 13), FGPIOB_PSOR); // 3
    writel((1 << 12), FGPIOA_PCOR); // 2

    /* Determine what percentage we'll adjust the note to */
    pct = voice->sustain_level + (voice->delay_time - voice->note_timer) * (voice->attack_level - voice->sustain_level) / (voice->delay_time - voice->attack_time);
  }
  else {
    /* Sustain phase */
    writel((1 << 7), FGPIOA_PCOR); // 5
    writel((1 << 0), FGPIOB_PSOR); // 4
    writel((1 << 13), FGPIOB_PCOR); // 3
    writel((1 << 12), FGPIOA_PCOR); // 2
    pct = voice->sustain_level;
  }

  /* Scale the note volume to the calcualted percentage */
  output = output * pct / 100;
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

#define VOICE_2_DELAY ((SAMPLE_RATE * 9) + ((SAMPLE_RATE * 1000) / 250))
void loop(void)
{

  // If a sample is still in the buffer, don't do anything.
  if (sample_queued) {
    return;
  }

  static uint32_t current_note;
  static uint32_t current_note_2;
  
  if (global_tick_counter >= (timesteps[current_note] << 5)) {
    
    if (velocities[current_note] == 0) {
      noteOff(&voice[0]);
      //noteOff(&voice[1]);
      digitalWrite(2, 0);
      current_note++;
    } else {
      noteOn(&voice[0], midi_freq[notes[current_note]]);
      //noteOn(&voice[1], frequencies[(current_note + 5) % 12]);
      digitalWrite(2, 1);
      current_note++;
    }
    if (current_note > sizeof(notes))
      current_note = 0;
  }

  if ((global_tick_counter > VOICE_2_DELAY) && ((global_tick_counter - VOICE_2_DELAY) >= (timesteps[current_note_2] << 5))) {
    
    if (velocities[current_note_2] == 0) {
      noteOff(&voice[1]);
      //noteOff(&voice[1]);
      current_note_2++;
    } else {
      noteOn(&voice[1], midi_freq[notes[current_note_2]]);
      //noteOn(&voice[1], frequencies[(current_note + 5) % 12]);
      current_note_2++;
    }
    if (current_note_2 > sizeof(notes))
      current_note_2 = 0;
  }

  next_sample = 0;
  next_sample += get_sample(&voice[0]);
  next_sample += get_sample(&voice[1]);
  sample_queued = 1;
}