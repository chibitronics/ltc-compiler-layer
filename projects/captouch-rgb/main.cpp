// LTC Cap touch test

#define pixelCount 5 // number of pixels in the chain; doesn't hurt to have less actual pixels in your project
#define dimLevel 1   // a level from 0-5

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, LED_BUILTIN_RGB, NEO_GRB + NEO_KHZ800);

void setup()
{

    strip.begin();
    strip.show();
}

void loop()
{
    int i;
    int level;
    unsigned long start_time;
    unsigned long end_time;

    // Set pin 1 high.
    pinMode(1, OUTPUT);
    digitalWrite(1, HIGH);
    // Wait a moment for it to charge.
    delay(2);

    // Set the pin back to an input and wait for it to change.
    start_time = micros();
    pinMode(1, INPUT);
    while (digitalRead(1))
        ;
    end_time = micros();

    level = end_time - start_time;

    for (i = 0; i < pixelCount; i++)
    {
        strip.setPixelColor(i, level | 0xFF00);
    }

    strip.show();
}
