#include "Arduino.h"
#include "ChibiOS.h"
void setup() {}
#if 0
#define PIN0_EFFECT FADE
#define PIN0_SPEED 5
#define PIN0_RANDOMNESS 0
#define PIN0_BRIGHTNESS 100

#define PIN1_EFFECT FADE
#define PIN1_SPEED 4
#define PIN1_RANDOMNESS 0
#define PIN1_BRIGHTNESS 100

#define PIN2_EFFECT FADE
#define PIN2_SPEED 3
#define PIN2_RANDOMNESS 0
#define PIN2_BRIGHTNESS 100

#define PIN3_EFFECT HEARTBEAT
#define PIN3_SPEED 8
#define PIN3_RANDOMNESS 0
#define PIN3_BRIGHTNESS 100

#define PIN4_EFFECT HEARTBEAT
#define PIN4_SPEED 5
#define PIN4_RANDOMNESS 0
#define PIN4_BRIGHTNESS 100

#define PIN5_EFFECT TWINKLE
#define PIN5_SPEED 5
#define PIN5_RANDOMNESS 0
#define PIN5_BRIGHTNESS 100

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
    {
        0,
        PIN0_EFFECT,
        PIN0_SPEED,
        PIN0_RANDOMNESS,
        PIN0_BRIGHTNESS,
    },
    {
        1,
        PIN1_EFFECT,
        PIN1_SPEED,
        PIN1_RANDOMNESS,
        PIN1_BRIGHTNESS,
    },
    {
        2,
        PIN2_EFFECT,
        PIN2_SPEED,
        PIN2_RANDOMNESS,
        PIN2_BRIGHTNESS,
    },
    {
        3,
        PIN3_EFFECT,
        PIN3_SPEED,
        PIN3_RANDOMNESS,
        PIN3_BRIGHTNESS,
    },
    {
        4,
        PIN4_EFFECT,
        PIN4_SPEED,
        PIN4_RANDOMNESS,
        PIN4_BRIGHTNESS,
    },
    {
        5,
        PIN5_EFFECT,
        PIN5_SPEED,
        PIN5_RANDOMNESS,
        PIN5_BRIGHTNESS,
    },
};

/////////////
///////////// implementation below
/////////////

static thread_t *light_threads[6];

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
    light_threads[i] = createThread(thread_areas[i], sizeof(thread_areas[i]),
                                    20,
                                    effects_thread,
                                    (void *)&pin[i]);
  }
}
#endif
void loop(void)
{
//  exitThread(0);
}
