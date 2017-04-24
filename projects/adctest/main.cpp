#include "Arduino.h"

void setup(void) {
  /*
  analogWrite(A0, 500);
  analogWrite(A1, 250);
  analogWrite(A2, 200);
  analogWrite(A3, 1020);
  */
}

void loop(void) {
  /*
  static int i;

  analogWrite(A0, (i/1 + 500) & 0x3ff);
  analogWrite(A1, (i/1 + 250) & 0x3ff);
  analogWrite(A2, (i/1 + 200) & 0x3ff);
  analogWrite(A3, (i/1 + 1020) & 0x3ff);

  i++;
  */

  printf("\r\nReading values...\r\n");
  printf("A0: %d\r\n", analogRead(A0));
  printf("A1: %d\r\n", analogRead(A1));
  printf("A2: %d\r\n", analogRead(A2));
  printf("A3: %d\r\n", analogRead(A3));
  printf("A4: %d\r\n", analogRead(A4));
  printf("A5: %d\r\n", analogRead(A5));
  printf("A6: %d\r\n", analogRead(A6));
  printf("A7: %d\r\n", analogRead(A7));
  printf("A8: %d\r\n", analogRead(A8));

  delayMicroseconds(50000);
}
