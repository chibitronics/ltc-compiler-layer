#include "Arduino.h"
#if 0

void setup(void) {
  int i;
  for (i = 0; i <= 5; i++)
    pinMode(i, OUTPUT);
}

#define LAR_MAX 256

/* We want the brightness to look like abs(1/x), but centered about the middle of
our range.  So if our range goes from 0 to 1000, we want the peak to be at 500.
*/

static int loop_to_larson(int i) {

  /* Center it between -512 and 512 */
  i = (i % 1024) - 512;
  i = 512 - abs(i);
  i -= 256;
  if (i < 0)
    return 0;

  return 8 * (8 - log2(i));
}

static int pleasant(int loop_count, int led_index) {
  return sin((float)loop_count + (float)led_index) / 60;
}

static int loop_count;
void loop(void) {
  loop_count++;

  int i;

    for (i = 0; i < 6; i++)
      //analogWrite(i, pleasant(loop_count, i));//sqrt(loop_to_larson(loop_count + (i * 10))));
      analogWrite(i, loop_to_larson(loop_count + (i * 10)));
      //analogWrite(i, (loop_count + (i * 60)) % 256);
/*
  analogWrite(1, (float)(loop_count + 20) / 10.0);
  analogWrite(2, (float)(loop_count + 40) / 10.0);
  analogWrite(3, (float)(loop_count + 60) / 10.0);
  analogWrite(4, (float)(loop_count + 80) / 10.0);
  analogWrite(5, (float)(loop_count + 10) / 10.0);
*/

  delay(5);
}
#else
#include "ChibiOS.h"

int blink[6] = {0,0,0,0,0,0};

#define FADE_RATE 4

static THD_WORKING_AREA(blinky_area, 64);
static THD_FUNCTION(do_blinky, arg) {
  (void)arg;
  int depth = 0;

  while (1) {
    if (blink[0]) {
      for (depth = 0; depth < 255; depth += FADE_RATE) {
        analogWrite(A0, depth);
        delay(10);
      }
      for (depth = 255; depth > 0; depth -= FADE_RATE) {
        analogWrite(A0, depth);
        delay(10);
      }
      blink[0] = 0;
    }
    analogWrite(A0, 0);
    delay(1);
  }
}

static THD_WORKING_AREA(blinky_area1, 64);
static THD_FUNCTION(do_blinky1, arg) {
  (void)arg;
  int depth = 0;

  while (1) {
    if( blink[1] ) {
      for( depth = 0; depth < 255; depth += FADE_RATE ) {
        analogWrite(A1, depth);
        delay(10);
      }
      for( depth = 255; depth > 0; depth -= FADE_RATE ) {
        analogWrite(A1, depth);
        delay(10);
      }
      blink[1] = 0;
    }
    analogWrite(A1, 0);
    delay(1);
  }
}

static THD_WORKING_AREA(blinky_area2, 64);
static THD_FUNCTION(do_blinky2, arg) {
  (void)arg;
  int depth = 0;

  while (1) {
    if( blink[2] ) {
      for( depth = 0; depth < 255; depth += FADE_RATE ) {
        analogWrite(A2, depth);
        delay(10);
      }
      for( depth = 255; depth > 0; depth -= FADE_RATE ) {
        analogWrite(A2, depth);
        delay(10);
      }
      blink[2] = 0;
    }
    analogWrite(A2, 0);
    delay(1);
  }
}

static THD_WORKING_AREA(blinky_area3, 64);
static THD_FUNCTION(do_blinky3, arg) {
  (void)arg;
  int depth = 0;

  while (1) {
    if( blink[3] ) {
      for( depth = 0; depth < 255; depth += FADE_RATE ) {
        analogWrite(A3, depth);
        delay(10);
      }
      for( depth = 255; depth > 0; depth -= FADE_RATE ) {
        analogWrite(A3, depth);
        delay(10);
      }
      blink[3] = 0;
    }
    analogWrite(A3, 0);
    delay(1);
  }
}

static THD_WORKING_AREA(blinky_area4, 64);
static THD_FUNCTION(do_blinky4, arg) {
  (void)arg;
  int depth = 0;

  while (1) {
    if( blink[4] ) {
      for( depth = 0; depth < 255; depth += FADE_RATE ) {
        analogWrite(A4, depth);
        delay(10);
      }
      for( depth = 255; depth > 0; depth -= FADE_RATE ) {
        analogWrite(A4, depth);
        delay(10);
      }
      blink[4] = 0;
    }
    analogWrite(A4, 0);
    delay(1);
  }
}

static THD_WORKING_AREA(blinky_area5, 64);
static THD_FUNCTION(do_blinky5, arg) {
  (void)arg;
  int depth = 0;

  while (1) {
    if( blink[5] ) {
      for (depth = 0; depth < 255; depth += FADE_RATE) {
        analogWrite(A5, depth);
        delay(10);
      }
      for (depth = 255; depth > 0; depth -= FADE_RATE) {
        analogWrite(A5, depth);
        delay(10);
      }
      blink[5] = 0;
    }
    analogWrite(A5, 0);
    delay(1);
  }
}


void setup(void) {
  pinMode(A0, OUTPUT);
  digitalWrite(A0, 0);

  pinMode(A1, OUTPUT);
  digitalWrite(A1, 0);

  pinMode(A2, OUTPUT);
  digitalWrite(A2, 0);

  pinMode(A3, OUTPUT);
  digitalWrite(A3, 0);

  pinMode(D0, OUTPUT);
  digitalWrite(D0, 0);

  pinMode(D1, OUTPUT);
  digitalWrite(D1, 0);

  createThread(blinky_area, sizeof(blinky_area), 20, do_blinky, NULL);
  createThread(blinky_area1, sizeof(blinky_area1), 20, do_blinky1, NULL);
  createThread(blinky_area2, sizeof(blinky_area2), 20, do_blinky2, NULL);
  createThread(blinky_area3, sizeof(blinky_area3), 20, do_blinky3, NULL);
  createThread(blinky_area4, sizeof(blinky_area4), 20, do_blinky4, NULL);
  createThread(blinky_area5, sizeof(blinky_area5), 20, do_blinky5, NULL);
}

void loop(void) {

  int j = 0;
  for (j = 0; j < 6; j++) {
    delay(500);
    blink[j] = 1;
  }
  for (j = 5; j >= 0; j--) {
    delay(500);
    blink[j] = 1;
  }
}
#endif
