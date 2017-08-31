/*
 * Chibi Palette template
 *
 * This program is the master template program used for the Chibi Chip Palette,
 * used to provide artists with an easy-to-use effects "palette".
 *
 * There are up to six effects, each if which is defined by a 5-byte struct.
 * In order to allow Javascript to simply search through the binary to assign
 * palette effects, we pre-populate this struct with magic numbers.
 */
#include "Arduino.h"
#include "ChibiOS.h"

#define PIN_MAGIC_NUMBER         \
  {                              \
    0x95, 0x32, 0xf3, 0x99, 0x2d \
  }

// Love to Code
// Effects Template
enum effects
{
  CONSTANT,
  FADE,
  HEARTBEAT,
  TWINKLE,
};

struct effects_thread_arg
{
  uint8_t pin;

  // Must be one of CONSTANT, FADE, HEARTBEAT, or TWINKLE
  uint8_t effect;

  // Speed goes from 1 to 25 (higher is faster)
  uint8_t speed;

  // Randomness from 0 to 100 (100 is more random)
  uint8_t randomness;

  // Brightness from 0 to 100 (100 is brighter)
  int8_t brightness;
};

static const effects_thread_arg pin[6] = {
    PIN_MAGIC_NUMBER,
    PIN_MAGIC_NUMBER,
    PIN_MAGIC_NUMBER,
    PIN_MAGIC_NUMBER,
    PIN_MAGIC_NUMBER,
    PIN_MAGIC_NUMBER,
};

/////////////
///////////// implementation below
/////////////

// Stack working area for each thread.
static stkalign_t thread_areas[6][96 / sizeof(stkalign_t)];

static int fade_to(int current, int target, int rate, int pin, int pause, int randomness, int brightness)
{
  int r;
  while (abs(current - target) > rate)
  {
    analogWrite(pin, map(current, 0, 255, 0, brightness));
    current = current + ((target - current) > 0 ? rate : -rate);
    r = random(0, 100);
    if (r < randomness)
    {
      if (r % 2)
      {
        if (pause - 1 > 0)
          delay(pause - 1);
      }
      else
      {
        delay(pause + 1);
      }
    }
    else
    {
      delay(pause);
    }
  }
  current = target; // handle cases where target and rate aren't multiples of 255
  analogWrite(pin, current);
  return current;
}

static void blink_effect(struct effects_thread_arg *cfg)
{
  fade_to(0, 255, cfg->speed, cfg->pin, 7, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
  fade_to(255, 0, cfg->speed, cfg->pin, 7, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
}

static void twinkle_effect(struct effects_thread_arg *cfg)
{
  int current = 128;
  while (1)
  {
    current = fade_to(current, random(0, 255), cfg->speed, cfg->pin, 3, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
  }
}

static void heartbeat_effect(struct effects_thread_arg *cfg)
{
  int current = 0;
  if (cfg->speed > 25)
    cfg->speed = 25;

  current = fade_to(current, 192, 2, cfg->pin, 1, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
  current = fade_to(current, 4, 2, cfg->pin, 1, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
  delay(40); // fastest rate
  delay((25 - cfg->speed) * 13 + 1);
  //delay(180);
  current = fade_to(current, 255, 2, cfg->pin, 1, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
  current = fade_to(current, 0, 2, cfg->pin, 1, cfg->randomness, map(cfg->brightness, 0, 100, 0, 255));
  digitalWrite(cfg->pin, 0);
  delay(107); // fastest rate
  delay((25 - cfg->speed) * 37 + 1);
  //delay(420);
}

static void effects_thread(void *arg)
{
  struct effects_thread_arg *cfg = (struct effects_thread_arg *)arg;
  while (1)
  {
    switch (cfg->effect)
    {
    case CONSTANT:
      exitThread(0);
      return;
    case FADE:
      blink_effect(cfg);
      break;
    case HEARTBEAT:
      heartbeat_effect(cfg);
      break;
    case TWINKLE:
      twinkle_effect(cfg);
      break;
    default:
      exitThread(0);
      return;
    }
  }
}

void setup(void)
{
  int i;

  for (i = 0; i < 6; i++)
  {
    pinMode(pin[i].pin, OUTPUT);
    digitalWrite(pin[i].pin, LOW);
    if (pin[i].effect == CONSTANT)
    {
      analogWrite(pin[i].pin, map(pin[i].brightness, 0, 100, 0, 255));
      continue;
    }

    delay(random(1, pin[i].randomness * 5) + 1);
    createThread(thread_areas[i], sizeof(thread_areas[i]),
                 20,
                 effects_thread,
                 (void *)&pin[i]);
  }
}

void loop(void)
{
  exitThread(0);
}
