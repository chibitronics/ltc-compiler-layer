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

static void set_led(int r, int g, int b, unsigned int led_number) {
  struct pixels pixels[3];
  unsigned int i;

  for (i = 0; i < ARRAY_SIZE(pixels); i++) {
    if (i == led_number) {
      pixels[i].r = r;
      pixels[i].g = g;
      pixels[i].b = b;
     }
     else {
      pixels[i].r = 0;
      pixels[i].g = 0;
      pixels[i].b = 0;
    }
  }

  ledShow(LED_BUILTIN_RGB, pixels, ARRAY_SIZE(pixels));
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
    printf("Connectivity test.  Enter 0-%d to test a particular pad, or 'q' to quit.\r\n", ARRAY_SIZE(test_pins));
    if (!cangetchar()) {
      delay(50);
      continue;
    }

    int c = getchar();

    if (c == 'q')
      break;

    if ((c - '0' >= 0) && (c - '0' <= 9)) {
      unsigned int pin = c - '0';
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

  while (1) {
    printf("PWM LED test.  Enter 0-5 to enable 50% PWM.  Enter 'q' to quit.\r\n");
    if (!cangetchar()) {
      delay(50);
      continue;
    }
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

  while (1) {
    printf("RGB LED test.  Enter 'r', 'g', 'b', 'R', 'G', 'B', or 'q' to quit\r\n");
    if (!cangetchar()) {
      delay(50);
      continue;
    }

    switch (c = getchar()) {
    case 'r':
      set_led(128, 0, 0, 0);
      break;
    case 'g':
      set_led(0, 128, 0, 0);
      break;
    case 'b':
      set_led(0, 0, 128, 0);
      break;
    case 'R':
      set_led(128, 0, 0, 2);
      break;
    case 'G':
      set_led(0, 128, 0, 2);
      break;
    case 'B':
      set_led(0, 0, 128, 2);
      break;
    case 'Q':
    case 'q':
      set_led(0, 0, 0, 1);
      return;
    }
  }
}

struct ltc_test {
  void (*function)(void);
  char shortcut[4];	// FOURCC code used to prevent noise from starting tests
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

// define this to turn on "fourcc", which uses four
// characters to define a command rather than one.
//#define USE_FOURCC

// define this to enable reduplication, where the
// command is repeated twice to ensure it's
// received correctly.
//#define REDUPLICATION

static const struct ltc_test ltc_tests[] = {
  {
    .function = test_leds,
#ifdef REDUPLICATION
    .shortcut = {'!','l','!','l'},
#else
    .shortcut = {'l','e','d','s'},
#endif
    .description = "White LED output test",
  },
  {
    .function = test_rgb,
#ifdef REDUPLICATION
    .shortcut = {'!','w','!','w'},
#else
    .shortcut = {'w','r','g','b'},
#endif
    .description = "Test WS2812b RGB LED",
  },
  {
    .function = test_serial,
#ifdef REDUPLICATION
    .shortcut = {'!','u','!','u'},
#else
    .shortcut = {'u','a','r','t'},
#endif
    .description = "Test serial",
  },
  {
    .function = test_connectivity,
#ifdef REDUPLICATION
    .shortcut = {'!','c','!','c'},
#else
    .shortcut = {'c','o','n','n'},
#endif
    .description = "Test MCU ball connectivity",
  },
  {
    .function = test_red_led,
#ifdef REDUPLICATION
    .shortcut = {'!','r','!','r'},
#else
    .shortcut = {'r','e','d','l'},
#endif
    .description = "Test red error status LED",
  },
  {
    .function = test_green_led,
#ifdef REDUPLICATION
    .shortcut = {'!','g','!','g'},
#else
    .shortcut = {'g','r','e','n'},
#endif
    .description = "Test green status LED",
  }
};

#ifdef USE_FOURCC
static int fourcc_matches(const char fourcc_buffer[4],
                          const char check_value[4],
                          uint8_t offset) {
  unsigned int i;
  for (i = 0; i < sizeof(fourcc_buffer); i++)
    if (fourcc_buffer[i] != check_value[(offset + i) & 3])
      return 0;
  return 1;
}
#endif

__attribute__((noreturn))
static void uart_menu(void) {
  uint32_t i;
  char fourcc_buffer[4] = {};
  uint8_t fourcc_ptr = 0;

  setSerialSpeed(9600);

  // Drain the UART buffer before printing the first banner.
  while (cangetchar())
    (void)getchar();

  while (1) {
    printf("LTC factory test is running.  Available tests:\r\n");
    for (i = 0; i < ARRAY_SIZE(ltc_tests); i++)
      printf("    %c"
#ifdef USE_FOURCC
"%c%c%c"
#endif
              ": %s\r\n",
              ltc_tests[i].shortcut[0],
#ifdef USE_FOURCC
              ltc_tests[i].shortcut[1],
              ltc_tests[i].shortcut[2],
              ltc_tests[i].shortcut[3],
#endif
              ltc_tests[i].description);
    delay(50);
    if (cangetchar()) {
#ifdef USE_FOURCC
      fourcc_buffer[fourcc_ptr++] = getchar();
      fourcc_ptr &= 3;
#else
      fourcc_buffer[fourcc_ptr] = getchar();
#endif

      for (i = 0; i < ARRAY_SIZE(ltc_tests); i++) {
#ifdef USE_FOURCC
        if (fourcc_matches(ltc_tests[i].shortcut, fourcc_buffer, fourcc_ptr)) {
#else
        if (ltc_tests[i].shortcut[0] == fourcc_buffer[fourcc_ptr]) {
#endif
          // Drain the UART buffer before running the test.
          while (cangetchar())
            (void)getchar();

          // Execute the test.
          ltc_tests[i].function();
          break;
        }
      }
    }
  }
}

__attribute__((noreturn))
void setup(void) {
  // Enable "sudo" mode, which gives us access
  // to all available pins.
  doSudo();

  // Ensure the LED is off.  The OS should have done this already,
  // but make sure anyway.
  set_led(0, 0, 0, 1);

  // Enter the UART "menu", which never returns.
  uart_menu();
}

void loop(void) {
  ;
}
