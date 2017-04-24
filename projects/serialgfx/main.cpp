#include "Arduino.h"

// One line is 18 characters long, but the \n counts as one of those.
static const uint8_t landscape_chars[16] = {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
                                         '*', '|', '_', '_', '!', };

static uint8_t landscape[12];
static void render_screen(uint32_t counter) {
  unsigned int i;

  printf("#SYN\n");
  printf("#LCK\n");

#if 1
  printf("/----------------\\");
  printf("| Hi?            |");
  printf("| Loops: %7d |", counter);

  printf("| [");
  for (i = 1; i < sizeof(landscape); i++) {
    landscape[i - 1] = landscape[i];
    Serial.write(landscape_chars[landscape[i - 1]]);
  }
  landscape[sizeof(landscape) - 1] = random(0, 16);
  Serial.write(landscape_chars[landscape[sizeof(landscape) - 1]]);
  printf("] |");

  printf("\\----------------/");
#else
printf("12345");
  printf("1:");
  for (i = 2; i < 18; i++)
    printf("%c", 'a' + ((1 + counter + i) % 26));

  printf("2:");
  for (i = 2; i < 18; i++)
    printf("%c", 'a' + ((2 + counter + i) % 26));

  printf("3:");
  for (i = 2; i < 18; i++)
    printf("%c", 'a' + ((3 + counter + i) % 26));

  printf("4:");
  for (i = 2; i < 18; i++)
    printf("%c", 'a' + ((4 + counter + i) % 26));

  printf("5:");
  for (i = 2; i < 18; i++)
    printf("%c", 'e' + ((counter + i) % 26));
#endif
}

void setup(void) {

  Serial.begin(9600);
}

static uint32_t counter = 0;
void loop(void) {

  render_screen(counter++);
  delay(500);
}
