#include "Arduino.h"
#include "HIDIO.h"

void setup() {
  HIDIO.begin();
}

void loop() {
  uint8_t record[8];
  static int loops;

  HIDIO.readWait(record, sizeof(record));
  record[0] = record[0] + 1;
  record[1] = record[0] + 2;
  record[2] = record[0] * 2;
  record[3] = 0x42;
  record[4] = loops;
  record[5] = loops >> 8;
  HIDIO.write(record, sizeof(record));
  loops++;
}
