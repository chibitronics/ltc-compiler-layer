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
  PTA(9), // ANA1(1)
  PTA(12), // ANA2
  PTB(13), // ANA3(1)
  PTB(0), // DIG0
  PTA(7), // DIG1
  PTB(10), // ANA0(2)
  PTB(11), // ANA1(2)
  PTB(4), // ANA3(2)
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

static void test_connectivity(void) {
  unsigned int i;

  // Mark every pin as an input.
  for (i = 0; i < ARRAY_SIZE(test_pins); i++)
    pinMode(test_pins[i], INPUT);

  while (1) {
    printf("Enter 0-%d to test a particular pad\r\n", ARRAY_SIZE(test_pins));
    int c = getchar();

    if (c == 'q')
      break;

    if ((c - '0' >= 0) && (c - '0' <= 9)) {
      int pin = c - '0';
      if (pin >= ARRAY_SIZE(test_pins))
        continue;
      printf("Testing pin %d (%c, 0x%02x) High", pin, c, test_pins[pin]);

      // Now wait for it to go high
      while (digitalRead(test_pins[pin]) != HIGH)
        delay(1);

      // Clear the green light, to acknowlege receipt
      printf("  Low");
      green_off();

      // Wait for it to go low again
      while (digitalRead(test_pins[pin]) != LOW)
        delay(1);

      printf("\r\n");
      // Light the green light, to finish this pin.
      green_on();
    }
  }
}

static void test_leds(void) {
  unsigned int i;
  int pin;

  printf("Enter 0-5 to enable 50% PWM.  Enter 'q' to quit.\r\n");

  while (1) {
    char c = getchar();

    switch (c) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
      pin = c - '0';

      // Deconfigure all pins first.
      for (i = 0; i <= 5; i++)
        pinMode(i, INPUT);

      // Create a 50% duty cycle, for the detector to see.
      pinMode(pin, OUTPUT);
      analogWrite(pin, 128);
      break;

    case 'q':
      // Deconfigure all pins just before exiting.
      for (i = 0; i <= 5; i++)
        pinMode(i, INPUT);

      return;
    }
  }
}

static void test_rgb(void) {
  int c;

  printf("Enter 'r', 'g', 'b', or 'q' to quit\r\n");

  while (1) {
    switch (c = getchar()) {
    case 'R':
    case 'r':
      set_led(128, 0, 0);
      break;
    case 'G':
    case 'g':
      set_led(0, 128, 0);
      break;
    case 'B':
    case 'b':
      set_led(0, 0, 128);
      break;
    case 'Q':
    case 'q':
      set_led(0, 0, 0);
      return;
    }
  }
}

struct ltc_test {
  void (*function)(void);
  char shortcut;
  const char *description;
};

static void test_red_led(void) {
  // Turn the LED on and wait for a response, then turn it off.
  red_on();
  getchar();
  red_off();
}

static void test_green_led(void) {
  // Turn the LED off (the bootloader turned it on).
  green_off();
  getchar();
  green_on();
}

static void test_serial(void) {
  while (1) {
    if (cangetchar()) {
      char c = getchar();
      if (c == 'q')
        return;
    }
    printf("LtC serial test\r\n");
    delay(50);
  }
}

static struct ltc_test ltc_tests[] = {
  {
    .function = test_leds,
    .shortcut = 'l',
    .description = "White LED output test",
  },
  {
    .function = test_rgb,
    .shortcut = 'w',
    .description = "Test WS2812b RGB LED",
  },
  {
    .function = test_serial,
    .shortcut = 's',
    .description = "Test serial",
  },
  {
    .function = test_connectivity,
    .shortcut = 'c',
    .description = "Test MCU ball connectivity",
  },
  {
    .function = test_red_led,
    .shortcut = 'r',
    .description = "Test red status LED",
  },
  {
    .function = test_green_led,
    .shortcut = 'g',
    .description = "Test green status LED",
  }
};

__attribute__((noreturn))
static void uart_menu(void) {
  uint32_t i;

  setSerialSpeed(9600);

  while (1) {
    if (cangetchar()) {
      char c = getchar();

      for (i = 0; i < ARRAY_SIZE(ltc_tests); i++) {
        if (ltc_tests[i].shortcut == c) {
          // Drain the UART buffer before running the test.
          while (cangetchar())
            (void)getchar();

          // Execute the test.
          ltc_tests[i].function();
          break;
        }
      }
    }
    printf("LTC factory test is running.  Available tests:\r\n");
    for (i = 0; i < ARRAY_SIZE(ltc_tests); i++)
      printf("    %c: %s\r\n",
              ltc_tests[i].shortcut,
              ltc_tests[i].description);
    delay(50);
  }
}

__attribute__((noreturn))
void setup(void) {
  // Enable "sudo" mode, which gives us access
  // to all available pins.
  doSudo();

  // Ensure the LED is off.  The OS should have done this already,
  // but make sure anyway.
  set_led(0, 0, 0);

  // Enter the UART "menu", which never returns.
  uart_menu();
}

void loop(void) {
  ;
}
