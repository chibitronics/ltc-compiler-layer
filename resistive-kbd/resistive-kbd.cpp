//  This example is Chibitronics Love to Code tested!
/*
 Resistive touch

 This example configures all the pins as inputs and waits for a touch
 stimulus from the pin to ground.

 Based on the touch detected, the NeoPixel LED will flash the number
 of times equal to the port number.

 25 May 2016
 by bunnie

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Debounce
 */

#include "Keyboard.h"

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers


int pinFocus = -2;  // the pin we're currently focused on in the debounce loop
int pinHeld = 0;

static const char inputs[] = {
  D0,
  D1,
  A0,
  A1,
  A2,
  //A3,
};

static const char keys[sizeof(inputs)] = {
  'A',
  'B',
  'C',
  'D',
  'E',
  //'F',
};

int pinState[sizeof(inputs)];

void setup(void) {
  unsigned int i;

  for (i = 0; i < sizeof(inputs); i++) {
    pinMode(inputs[i], INPUT);
    pinState[i] = HIGH;          // pins default to high
  }

  Keyboard.begin();
}

void updatePinState(void) {
  unsigned int i;

  for (i = 0; i < sizeof(inputs); i++) {
    pinState[i] = digitalRead(inputs[i]);
  }
}

// returns the lowest-number pin that is currently pressed
int whichPinPressed(void) {
  unsigned int i;

  for (i = 0; i < sizeof(inputs); i++) {
    if (pinState[i] != HIGH)
      return i;
  }

  // if none changed, return -1
  return -1;
}

void loop(void) {
  int whichPin;

  // function to read all pins and copy to an array
  updatePinState();

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  whichPin = whichPinPressed();
  if ( (whichPin != -1) && (pinFocus == -2)) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    pinFocus = whichPin;
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // if the current pin we're looking at is still the focus, then we've got a hit
    if (whichPin == pinFocus) {
      if (pinHeld == 0) {
        // press the key on the USB keyboard
        Keyboard.write(keys[pinFocus]);
        delay(200);
        pinHeld = 1;
      }
      else {
       // if pin is being held, do nothing until it's released...
      }
    }
    else {
      if (whichPin == -1) {
       pinFocus = -2;  // we're not looking at /any/ pin, reset pinHeld state
       pinHeld = 0;
      }
      else {
       // in this case we rolled over to another pin...need to let go of all pins before we can move on!
      }
    }
  }
}

