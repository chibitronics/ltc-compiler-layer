#include <stdint.h>
#include <string.h>
#include "wave-table.h"
#include "note-table.h"

//#define WRITE_TO_FILE
#define VOICE_COUNT 2

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))
#endif

#ifdef ARDUINO_APP
    extern "C" void errorCondition(void);
#define panic(x) do {                         \
    errorCondition();                         \
} while(0)
#endif
#ifdef DESKTOP
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define panic(x) do {                         \
    fprintf(stderr, "PANIC: %s\n", x);        \
    exit(1);                                  \
} while(0)
#endif


// Format:
// ____ ____ ____ ____
// 0DDD DDPP PPPn nnnn
// |  |    |        \- Note offset from current "base".
// |  |    \---------- Duration (in ticks) of the post-note pause
// |  \--------------- Duration (in ticks) of this note
// \------------------ Word type: 0 for normal note, 1 for effect
//
// 1000 eeee aaaa aaaa
// |     |      \----- Argument to the effect
// |     \------------ Effect number
// \------------------ 1 indicates effect
//
// 1001 tttt tttt tttt - Set global tick-per-loop counter
//        \------------- Ticks per loop
//
// 1010 tttt tttt tttt - Set voice attack time
//            \--------- Time (in ticks)
//
// 1011 tttt tttt tttt - Set voice decay time
//            \--------- Time (in ticks)
//
// 1100 tttt tttt tttt - Set voice release time
//            \--------- Time (in ticks)

#define N_32 0
#define N_16 1
#define N_8 2
#define N_EIGHTH N_8
#define N_4 4
#define N_QUARTER N_4
#define N_2 8
#define N_HALF N_2
#define N_DOTTED_HALF 12
#define N_1 16
#define N_WHOLE N_1

enum ltc_pattern_effect {
    // Delay the next instruction by this amount
    DELAY_TICKS = 1,

    // Jump to a new pattern
    PATTERN_JUMP_ABS = 2,

    // Set the current voice's instrument
    SET_INSTRUMENT = 3,

    // Set the current voice's attack level
    SET_ATTACK_LEVEL = 4,

    // Set the current voice's attack level
    SET_DECAY_LEVEL = 5,

    // Set the current voice's sustain level
    SET_SUSTAIN_LEVEL = 6,

    /// Sets note 0's offset
    SET_NOTE_OFFSET = 7,

    /// Jump a relative number of patterns forward or backwards
    PATTERN_JUMP_REL = 8,

    /// Repeat the current pattern this many times
    PATTERN_REPEAT_COUNT = 9,

    FINAL_EFFECT = 10,
};

enum adsr_phase {
    PHASE_OFF,
    PHASE_ATTACK,
    PHASE_DECAY,
    PHASE_SUSTAIN,
    PHASE_RELEASE,
};

#ifdef DEBUG_ADSR
static void print_phase(int i) {
    if (i == PHASE_ATTACK) fprintf(stderr, "PHASE_ATTACK");
    else if (i == PHASE_DECAY) fprintf(stderr, "PHASE_DECAY");
    else if (i == PHASE_SUSTAIN) fprintf(stderr, "PHASE_SUSTAIN");
    else if (i == PHASE_RELEASE) fprintf(stderr, "PHASE_RELEASE");
    else if (i == PHASE_OFF) fprintf(stderr, "PHASE_OFF");
    else fprintf(stderr, "UNKNOWN (%d)", i);
}

#define ADSR_PHASE(v, p) do { \
v->phase_timer = 0; \
print_phase(v->adsr_phase); \
fprintf(stderr, " -> "); \
v->adsr_phase = p; \
print_phase(v->adsr_phase); \
fprintf(stderr, "\n"); \
} while(0)
#else /* !DEBUG_PHASE */
#define ADSR_PHASE(v, p) do { \
v->phase_timer = 0; \
v->adsr_phase = p; \
} while(0)
#endif /* DEBUG_PHASE */


#define NN(note, duration, pause) (((((note)+16) & 0x1f)) \
                                | (((duration) << 10) & (0x1f << 10)) \
                                | (((pause) << 5) & (0x1f << 5)) )
#define NE(effect, arg) ((((effect) & 0x7f) << 8) | (((arg) & 0xff) << 0) | (1 << 15))
#define NGT(time) (0x9000 | (time & 0xfff)) // Set global tick counter
#define NAT(time) (0xa000 | (time & 0xfff)) // Voice attack time
#define NDT(time) (0xb000 | (time & 0xfff)) // Voice decay time
#define NRT(time) (0xc000 | (time & 0xfff)) // Voice release time

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
#define SAMPLE_RATE (187392/12)

// The number of ticks that the sound system has gone through.
// Overflows after about three days, at 14 kHz.
volatile uint32_t global_tick_counter;

// The next sample to be played, nominally between -128 and 127
static int32_t next_sample;

// Nonzero if a sample has been queued, zero if the sample buffer is empty.
static volatile uint8_t sample_queued = 0;

static const struct ltc_instrument *instruments[] = {
    &triangle_instrument,
    &sawtooth_instrument,
    &sine_instrument,
    &square_instrument,
};

// An ltc voice
struct ltc_voice
{
    /// The current note's frequency.
    uint32_t frequency;

    /// A pointer to the currently-selected instrument.
    const struct ltc_instrument *instrument;

    /// How long the Attack phase is
    uint32_t attack_time;

    /// How strong the Attack phase starts
    uint8_t attack_level;

    /// How strong the Decay phase starts (and the Attack phase ends)
    uint8_t decay_level;

    /// How long the Decay time is
    uint32_t decay_time;

    /// How strong the Sustain phase is (after the Decay phase ends)
    uint8_t sustain_level;

    /// How long the Release phase is (i.e. after the note has ended)
    uint32_t release_time;

    // Keeps track of the phase in the instrument at the
    // given frequency.
    uint32_t phase_accumulator;

    /// How far into the current phase are we
    uint32_t phase_timer;

    // A pointer to the currently-operating pattern
    const uint16_t *pattern;
    uint16_t pattern_num;
    uint16_t pattern_offset;
    uint8_t pattern_repeat_count;

    /// The number of ticks left until we issue a Release.
    uint32_t note_duration;

    /// After the note, there is a period of time to wait for the next note.
    uint32_t rest_duration;

    /// All notes are relative to this note.
    uint8_t note_offset;

    /// 0: off
    /// 1: attack
    /// 2: decay
    /// 3: sustain
    /// 4: release
    uint8_t adsr_phase;
};

static const uint16_t voice0_setup[] = {
    NGT(200),
    NE(SET_INSTRUMENT, 3),

    NAT(40),
    NE(SET_ATTACK_LEVEL, 20),
    NDT(50),
    NE(SET_DECAY_LEVEL, 70),
    NE(SET_SUSTAIN_LEVEL, 30),
    NRT(20),

    NE(PATTERN_JUMP_ABS, 2),
};

static const uint16_t voice1_setup[] = {
    NE(SET_INSTRUMENT, 3),
    NAT(70),
    NE(SET_ATTACK_LEVEL, 60),
    NDT(50),
    NE(SET_DECAY_LEVEL, 30),
    NE(SET_SUSTAIN_LEVEL, 60),
    NRT(100),

    NE(DELAY_TICKS, 1),
    NE(PATTERN_JUMP_ABS, 2),
};

#include "nyan.h"

static const uint16_t *sample_song_patterns[] = {
    voice0_setup,
    voice1_setup,
    SONG_PATTERNS
};

struct ltc_song {
    const uint16_t **patterns;
    const uint8_t pattern_count;
};

static const struct ltc_song sample_song = {
    .patterns = sample_song_patterns,
    .pattern_count = ARRAY_SIZE(sample_song_patterns),
};

struct ltc_sound_engine {
    struct ltc_voice voices[VOICE_COUNT];

    // Global counter
    uint32_t tick_counter;

    // Number of loops-per-tick
    uint32_t loops_per_tick;

    // Currently-selected song
    const struct ltc_song *song;
};

static struct ltc_sound_engine engine;

static void patternDelay(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    engine->voices[channel].rest_duration = arg * engine->loops_per_tick;
}

static void patternJumpAbs(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    if (arg >= engine->song->pattern_count) {
        panic("attempt to abs jump to nonexistent pattern");
    }
    engine->voices[channel].pattern = engine->song->patterns[arg];
    engine->voices[channel].pattern_num = arg;
    engine->voices[channel].pattern_offset = 0;
    engine->voices[channel].pattern_repeat_count = 0;
}

static void patternJumpRel(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    int8_t target_num = (int8_t)engine->voices[channel].pattern_num + (int8_t)arg;
    if (target_num >= engine->song->pattern_count) {
        panic("attempt to rel jump to nonexistent pattern");
    }
    if (target_num < 0) {
        panic("attempt to jump to nonexistent pattern < 0");
    }
    engine->voices[channel].pattern = engine->song->patterns[target_num];
    engine->voices[channel].pattern_num = target_num;
    engine->voices[channel].pattern_offset = 0;
    engine->voices[channel].pattern_repeat_count = 0;
}

static void setInstrument(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    if (arg >= ARRAY_SIZE(instruments))
        panic("instrument is out of range");
    engine->voices[channel].instrument = instruments[arg];
}

static void setAttackTime(struct ltc_sound_engine *engine, uint8_t channel, uint16_t arg)
{
    engine->voices[channel].attack_time = (arg * SAMPLE_RATE) / 1000;
}

static void setAttackLevel(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    engine->voices[channel].attack_level = arg;
}

static void setDecayLevel(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    engine->voices[channel].decay_level = arg;
}

static void setDecayTime(struct ltc_sound_engine *engine, uint8_t channel, uint16_t arg)
{
    engine->voices[channel].decay_time = (arg * SAMPLE_RATE) / 1000;
}

static void setSustainLevel(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg)
{
    engine->voices[channel].sustain_level = arg;
}

static void setReleaseTime(struct ltc_sound_engine *engine, uint8_t channel, uint16_t arg)
{
    engine->voices[channel].release_time = (arg * SAMPLE_RATE) / 1000;
}

static void setGlobalSpeed(struct ltc_sound_engine *engine, uint8_t channel, uint16_t arg)
{
    (void)channel;
    engine->loops_per_tick = arg;
}

static void setNoteOffset(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg) {
    engine->voices[channel].note_offset = arg;
}

static void patternRepeatCount(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg) {
    struct ltc_voice *voice = &engine->voices[channel];
    int new_count = voice->pattern_repeat_count - 1;

    // If pattern_repeat_count is nonzero, then we're in the middle of repeating.
    switch (voice->pattern_repeat_count) {
    // If it's 1, then it's a NOP, since we've already processed it
    // during this iteration of a pattern.  Must jump to a new pattern
    // first.
    case 1:
        break;
    case 0:
        new_count = arg - 1;
        /* Fall through */
    default:
        // Repeat the pattern
        patternJumpRel(engine, channel, 0);

        // Update pattern_repeat_count, which is cleared as part of the jump.
        voice->pattern_repeat_count = new_count;

        break;
    }
}

typedef void (*effect_t)(struct ltc_sound_engine *engine, uint8_t channel, uint8_t arg);

static const effect_t effect_lut[] = {
    0,
    patternDelay,
    patternJumpAbs,
    setInstrument,
    setAttackLevel,
    setDecayLevel,
    setSustainLevel,
    setNoteOffset,
    patternJumpRel,
    patternRepeatCount,
};

void setSong(struct ltc_sound_engine *engine, const struct ltc_song *song) {
    int voice_num;
    engine->song = song;

    for (voice_num = 0; voice_num < VOICE_COUNT; voice_num++) {
        struct ltc_voice *voice = &engine->voices[voice_num];
        voice->pattern = song->patterns[voice_num];
        voice->pattern_num = voice_num;
        voice->pattern_offset = 0;
        voice->pattern_repeat_count = 0;
        voice->note_duration = 0;
        voice->rest_duration = 0;
        voice->sustain_level = 100;
        ADSR_PHASE(voice, PHASE_OFF);
        voice->instrument = 0;
        voice->note_offset = 40;
    }
}

#define ATTACK_PHASE 1
#define DECAY_PHASE 2
#define SUSTAIN_PHASE 3
#define RELEASE_PHASE 4
#ifdef DEBUG
#define DEBUG_PHASE(x) do { \
writel((1 << 12), (x == ATTACK_PHASE) ? FGPIOA_PSOR : FGPIOA_PCOR); \
writel((1 << 13), (x == DECAY_PHASE) ? FGPIOB_PSOR : FGPIOB_PCOR); \
writel((1 << 0), (x == SUSTAIN_PHASE) ? FGPIOB_PSOR : FGPIOB_PCOR); \
writel((1 << 7), (x == RELEASE_PHASE) ? FGPIOA_PSOR : FGPIOA_PCOR); \
} while(0);
#else
#define DEBUG_PHASE(x)
#endif
static int32_t processADSR(struct ltc_voice *voice, int32_t output)
{
    int32_t pct;

    voice->phase_timer++;
    switch (voice->adsr_phase) {
        /* For the ATTACK phase, the level starts at at voice->attack_level
         * and ends at voice->decay_level, during the course of voice->attack_time.
         */
        case PHASE_ATTACK:
            if (voice->attack_time) {
                /* Function progress:
                 *     t = 0             pct = voice->attack_level
                 *     t = attack_time   pct = voice->decay_level
                 */
                /* Determine what percentage we'll adjust the note to */
                pct = ((int32_t)voice->phase_timer * ((int32_t)voice->decay_level - (int32_t)voice->attack_level) / (int32_t)voice->attack_time) + (int32_t)voice->attack_level;
if (pct > 100)
panic("Percentage is > 100");
//fprintf(stderr, "attack_level: %d  decay_level: %d  phase_timer: %d  attack_time: %d  pct: %d\n",
//voice->attack_level, voice->decay_level, voice->phase_timer, voice->attack_time, pct);

                if (voice->phase_timer >= voice->attack_time) {
                    ADSR_PHASE(voice, PHASE_DECAY);
                }
                break;
            }
            /* Fall through if attack_time is 0 */
            ADSR_PHASE(voice, PHASE_DECAY);

        case PHASE_DECAY:
            if (voice->decay_time) {
                /* Determine what percentage we'll adjust the note to */
                pct = ((int32_t)voice->phase_timer * ((int32_t)voice->sustain_level - (int32_t)voice->decay_level) / (int32_t)voice->decay_time) + voice->decay_level;
//fprintf(stderr, "decay_level: %d  sustain_level: %d  phase_timer: %d  decay_time: %d  pct: %d\n",
//voice->decay_level, voice->sustain_level, voice->phase_timer, voice->decay_time, pct);
                if (voice->phase_timer >= voice->decay_time) {
                    ADSR_PHASE(voice, PHASE_SUSTAIN);
                }
                break;
            }
            /* Fall through if decay_time is 0 */
            ADSR_PHASE(voice, PHASE_SUSTAIN);

        case PHASE_SUSTAIN:
            pct = voice->sustain_level;
            break;

        case PHASE_RELEASE:
            if (voice->release_time) {
                /* Determine what percentage we'll adjust the note to */
                pct = ((int32_t)voice->release_time - (int32_t)voice->phase_timer) * (int32_t)voice->sustain_level / (int32_t)voice->release_time;
                if (voice->phase_timer >= voice->release_time) {
                     ADSR_PHASE(voice, PHASE_OFF);
                }
                break;
            }
            /* Fall through if release_time is 0 */
            ADSR_PHASE(voice, PHASE_OFF);

        case PHASE_OFF:
        default:
            pct = 0;
            break;
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

    if (!voice->instrument)
        return 0;

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
    }
    else
    {
        output = voice->instrument->samples[position];
    }

    output = processADSR(voice, output);

    return output;
}

static void note_on(struct ltc_voice *voice, uint32_t freq)
{
    voice->frequency = freq;
    voice->phase_accumulator = 0;
    ADSR_PHASE(voice, PHASE_ATTACK);
}

static void note_off(struct ltc_voice *voice)
{
    voice->phase_timer = 0;
    if (voice->adsr_phase != PHASE_OFF)
        ADSR_PHASE(voice, PHASE_RELEASE);
}

static void play_routine_step(struct ltc_sound_engine *engine) {
    int voice_num;
    for (voice_num = 0; voice_num < VOICE_COUNT; voice_num++) {
        struct ltc_voice *voice = &engine->voices[voice_num];
        if ((voice->note_duration == 0) && (voice->rest_duration == 0)) {
            uint16_t op = voice->pattern[voice->pattern_offset++];
            if ((op & 0xf000) == 0x8000) {
                uint32_t effect_num = (op >> 8) & 0x7f;
                if (effect_num > ARRAY_SIZE(effect_lut)) {
                    panic("effect_num out of range");
                }
                effect_lut[effect_num](engine, voice_num, op & 0xff);
            }
            else if ((op & 0xf000) == 0x9000) {
                setGlobalSpeed(engine, voice_num, op & 0xfff);
            }
            else if ((op & 0xf000) == 0xa000) {
                setAttackTime(engine, voice_num, op & 0xfff);
            }
            else if ((op & 0xf000) == 0xb000) {
                setDecayTime(engine, voice_num, op & 0xfff);
            }
            else if ((op & 0xf000) == 0xc000) {
                setReleaseTime(engine, voice_num, op & 0xfff);
            }
            else {
                uint32_t note_duration = (op >> 10) & 0x1f;
                uint32_t rest_duration = (op >> 5) & 0x1f;
                uint32_t note_index = ((op >> 0) & 0x1f) - 16;
                note_index = voice->note_offset + note_index;
                voice->note_offset = note_index;

                if (note_index > ARRAY_SIZE(note_lut))
                    panic("note_index out of range");
                note_on(voice, note_lut[note_index]);

                voice->note_duration = note_duration * engine->loops_per_tick;
                voice->rest_duration = rest_duration * engine->loops_per_tick;
            }
        }
        else if (voice->note_duration) {
            voice->note_duration--;
            if (!voice->note_duration)
                note_off(voice);
        }
        else if (voice->rest_duration) {
            voice->rest_duration--;
        }
    }
}

#ifdef ARDUINO_APP

#include "Arduino.h"
#include "ChibiOS.h"
#include "kl02.h"
#include "memio.h"

static int pwm0_loops = 0;
static int other_loops;
static int pwm0_stable_timer(void)
{

    pwm0_loops++;
    if (pwm0_loops > PWM_DELAY_LOOPS)
    {
        int32_t scaled_sample = next_sample + 129;
        if (scaled_sample > 255)
            scaled_sample = 255;
        if (scaled_sample < 1)
            scaled_sample = 1;
        writel(scaled_sample, TPM0_C1V);
        writel(scaled_sample, TPM0_C0V);

        pwm0_loops = 0;
        sample_queued = 0;
        //global_tick_counter++;
    }

    if (other_loops++ > 12) {
        global_tick_counter++;
        other_loops = 0;
    }

    /* Reset the timer IRQ, to allow us to fire again next time */
    writel(TPM0_STATUS_CH1F | TPM0_STATUS_CH0F | TPM0_STATUS_TOF, TPM0_STATUS);

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
#endif

void setup(void)
{
    setSong(&engine, &sample_song);
#ifdef ARDUINO_APP
    prepare_pwm();
    enableInterrupt(PWM0_IRQ);
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
#endif
}

void loop(void)
{
    uint32_t voice_num;
    // If a sample is still in the buffer, don't do anything.
    if (sample_queued)
        return;

    play_routine_step(&engine);

    next_sample = 0;
    for (voice_num = 0; voice_num < VOICE_COUNT; voice_num++)
        next_sample += get_sample(&engine.voices[voice_num]);
    sample_queued = 1;

#ifdef DESKTOP
    {
#ifdef WRITE_TO_FILE
        static FILE *outfile;
        if (!outfile)
            outfile = fopen("song.raw", "wb");
#endif
        int32_t scaled_sample = next_sample + 129;
        if (scaled_sample > 255)
            scaled_sample = 255;
        if (scaled_sample < 1)
            scaled_sample = 1;
#ifdef WRITE_TO_FILE
        fputc(scaled_sample, outfile);
        if (global_tick_counter >= 524288) {
            fflush(outfile);
            exit(0);
        }
#else
        fputc(scaled_sample, stdout);
        fflush(stdout);
#endif
        sample_queued = 0;
    }
#endif
}

#ifdef DESKTOP
int main(int argc, char **argv) {
    setup();
    while (1) {
        loop();
        global_tick_counter++;
    }
    return 0;
}
#endif
