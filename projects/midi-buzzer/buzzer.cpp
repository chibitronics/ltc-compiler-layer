/*
 * MIDIUSB_buzzer.ino
 *
 * Author: Paulo Costa
 */ 

#include <MIDIUSB.h>
#include "pitchToFrequency.h"

#define BUZZ_PIN 0

const char* pitch_name(byte pitch) {
  static const char* names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
  return names[pitch % 12];
}

int pitch_octave(byte pitch) {
  return (pitch / 12) - 1;
}

void noteOn(byte channel, byte pitch, byte velocity) {
  tone(BUZZ_PIN, pitchFrequency[pitch]);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  noTone(BUZZ_PIN);
}

void controlChange(byte channel, byte control, byte value) {
}

void setup() {
}

void loop() {
  midiEventPacket_t rx = MidiUSB.read();
  switch (rx.header) {
    case 0:
      break; //No pending events
      
    case 0x9:
      noteOn(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //pitch
        rx.byte3         //velocity
      );
      break;
      
    case 0x8:
      noteOff(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //pitch
        rx.byte3         //velocity
      );
      break;
      
    case 0xB:
      controlChange(
        rx.byte1 & 0xF,  //channel
        rx.byte2,        //control
        rx.byte3         //value
      );
      break;
  }
}

