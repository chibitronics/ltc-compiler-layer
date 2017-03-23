#include "Arduino.h"
#include "ChibiOS.h"
extern "C" void doSudo(void);
extern "C" void ledShow(uint32_t pin, void *pixels, uint32_t num_leds);

// The status LED will go "Green" when programming
// has completed.  This will signal to the tester
// that we're ready to go.
// The first thing we should do is test the input,
// which involves setting everything to "input"
// and waiting for all test pins to go high.

uint32_t test_pins[] = {
  PTA(8), // ANA0(1)
  PTB(10), // ANA0(2)
  PTA(9), // ANA1(1)
  PTB(11), // ANA1(2)
  PTA(12), // ANA2
  PTB(13), // ANA3(1)
  PTB(4), // ANA3(2)
  PTB(0), // DIG0
  PTA(7), // DIG1
};
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

struct pixels {
  uint8_t g;
  uint8_t r;
  uint8_t b;
} __attribute__((packed));

static void set_led(int r, int g, int b) {
  struct pixels pixel;

  pixel.r = r;
  pixel.g = g;
  pixel.b = b;
  ledShow(LED_BUILTIN_RGB, &pixel, 1);
}

static void green_on(void) {
    digitalWrite(LED_BUILTIN_GREEN, 0);
}

static void green_off(void) {
    digitalWrite(LED_BUILTIN_GREEN, 1);
}

static void red_on(void) {
    digitalWrite(LED_BUILTIN_RED, 0);
}

static void red_off(void) {
    digitalWrite(LED_BUILTIN_RED, 1);
}

static void test_user_pins(void) {
  unsigned int i;

  // Mark every pin as an input.
  for (i = 0; i < ARRAY_SIZE(test_pins); i++) {
    pinMode(test_pins[i], INPUT);
  }

  // Light up the red LED, so the controller knows to continue.
  red_on();

  // Wait for each pin to go low->high->low.
  for (i = 0; i < ARRAY_SIZE(test_pins); i++) {

    // Light up the green LED, to indicate we're
    // ready for another pin.

    // Now wait for it to go high
    while (digitalRead(test_pins[i]) != HIGH)
      delay(1);

    // Clear the green light, to acknowlege receipt
    green_off();

    // Wait for it to go low again
    while (digitalRead(test_pins[i]) != LOW)
      delay(1);

    // Light the green light, to finish this pin.
    green_on();
  }

  // Wait for pin 4 to go "high", indicating it's done.
  pinMode(4, INPUT);
  while (digitalRead(4) != HIGH)
    delay(1);
}

static void test_uart(void) {
  printf("starting loop\r\n");
  setSerialSpeed(9600);

  while (1) {
    if (cangetchar())
      if (getchar() == 'q')
        break;
    printf("test-running\r\n");
    delay(500);
  }
}

static void test_leds(void) {
  unsigned int i;

  for (i = 0; i < 5; i++) {
    // We receive a response on the next pin.
    // Set it to an input, and wait for it to go low.
    pinMode((i + 1) % 5, INPUT);
    while (digitalRead((i + 1) % 5) != LOW)
      delay(1);

    // Send a 50% duty cycle out and wait for response.
    pinMode(i, OUTPUT);
    analogWrite(i, 128);

    // Response happens when the pin goes high.
    while (digitalRead((i + 1) % 5) != HIGH)
      delay(1);

    // Turn the pin to an input, so we don't drive it anymore
    pinMode(i, INPUT);
  }
}

static void test_rgb(void) {
  static const int signal_pin = 1;
  pinMode(signal_pin, INPUT);

  // Waot for pin 0 to go high, indicating we can test.
  while (digitalRead(signal_pin) != HIGH)
      delay(1);
  set_led(255, 0, 0);
  while (digitalRead(signal_pin) != LOW)
      delay(1);

  while (digitalRead(signal_pin) != HIGH)
      delay(1);
  set_led(0, 255, 0);
  while (digitalRead(signal_pin) != LOW)
      delay(1);

  while (digitalRead(signal_pin) != HIGH)
      delay(1);
  set_led(0, 0, 255);
  while (digitalRead(signal_pin) != LOW)
      delay(1);

  set_led(0, 0, 0);
}

void setup(void) {
  // Enable "sudo" mode, which gives us access
  // to all available pins.
  doSudo();

  set_led(0, 0, 0);
  test_uart();

  test_user_pins();

  test_leds();

  test_rgb();
}

void loop(void) {
  ;
}
