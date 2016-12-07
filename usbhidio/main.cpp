#include "Arduino.h"
#include "HIDIO.h"
#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, LED_BUILTIN_RGB,
                                            NEO_GRB + NEO_KHZ800);
int loops;

void setup() {
  HIDIO.begin();
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

enum command {
  read_digital = 0,
  write_digital = 1,
  read_analog = 2,
  write_analog = 3,
  read_serial = 4,
  write_serial = 5,
  set_rgb = 6,
};

void loop() {
  uint8_t record[8];

  HIDIO.readWait(record, sizeof(record));

  switch ((enum command)(record[0])) {
  case read_digital:
    record[3] = digitalRead(record[1]);
    break;

  case write_digital:
    digitalWrite(record[1], record[2]);
    break;

  case read_analog:
    record[3] = analogRead(record[1]);
    break;

  case write_analog:
    analogWrite(record[1], record[2]);
    break;

  case read_serial:
    record[3] = Serial.read();
    break;

  case write_serial:
    Serial.write(record[2]);
    break;

  case set_rgb:
    strip.setPixelColor(0, record[1], record[2], record[3]);
    strip.show();
    break;

  default:
    break;
  }

  record[4] = 0;
  record[5] = 0;
  record[6] = loops;
  record[7] = loops >> 8;
  HIDIO.write(record, sizeof(record));
  loops++;
}
