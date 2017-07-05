// Love to Code

// Resistive touch to keyboard demo. The Chibi Chip becomes a virtual
// keyboard, mapping pins to key presses.
//
// To press a pin, hold +3V on the far right with one hand, and press
// any pin with your other hand. Your body completes the circuit, triggering
// a key press event!

#include "Arduino.h"
#include "Keyboard.h"

// Setup your key mapping here. Each line corresponds to a key,
// from pin 0 through 5 on the Chibi Chip
// The below mapping will make pins 0-3 press the keys 0-3, while
// pin 4 is the space bar and pin 5 is enter
static const char keys[6] = {
    '0',
    '1',
    '2',
    '3',
    ' ', // space key
    KEY_RETURN,
};

/*
    // example of using the special key codes below. Don't use the quote character with them!
    // The below mapping will make pins 0-3 be left, up, down, and right arrows respectively, while
    // pin 4 is the space bar and pin 5 is enter
    static const char keys[6] = {
      KEY_LEFT_ARROW,
      KEY_UP_ARROW,
      KEY_DOWN_ARROW,
      KEY_RIGHT_ARROW,
      ' ',  // space key
      KEY_RETURN,
    };
    */

/* some more possible key codes:
    KEY_UP_ARROW
    KEY_DOWN_ARROW
    KEY_LEFT_ARROW
    KEY_RIGHT_ARROW
     
    KEY_LEFT_CTRL
    KEY_LEFT_SHIFT   
    KEY_LEFT_ALT       
    KEY_LEFT_GUI       
    KEY_RIGHT_CTRL   
    KEY_RIGHT_SHIFT  
    KEY_RIGHT_ALT   
    KEY_RIGHT_GUI      
    KEY_BACKSPACE      
    KEY_TAB    
    KEY_RETURN                 
    KEY_ESC    
    KEY_INSERT                 
    KEY_DELETE                 
    KEY_PAGE_UP               
    KEY_PAGE_DOWN      
    KEY_HOME
    KEY_END    
    KEY_CAPS_LOCK   
           
    KEY_F1       
    KEY_F2       
    KEY_F3       
    KEY_F4       
    KEY_F5       
    KEY_F6       
    KEY_F7       
    KEY_F8       
    KEY_F9       
    KEY_F10
    KEY_F11    
    KEY_F12 
     
    */

// How many samples to take for the initialisation of the thresholds.
#define initialAverageCount 10

// Number of samples for press detection.
#define sampleLoopSize 5

// Time threshold for capacitive touch. Longer discharge time than this means there's a touch.
int threshold[6];

long lastDebounceTime = 0;
long debounceDelay = 50;

int pinFocus = -2; // the pin we're currently focused on in the debounce loop
int pinHeld = 0;
int lastFocus = 0;

int pinState[6];

int get_cap_level(int pin)
{
    unsigned long start_time;
    unsigned long end_time;

    // Set pin high.
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);

    // Wait a moment for it to charge.
    delayMicroseconds(100);

    // Set the pin back to an input and wait for it to change low again.
    start_time = micros();
    pinMode(pin, INPUT);
    while (digitalRead(pin))
        ;
    end_time = micros();

    // Report how long it took.
    return end_time - start_time;
}

void setup()
{
    int i, j;
    unsigned long accumulator;

    Keyboard.begin();

    // Average a bunch of baseline readings so we can
    // work out what our board's normal capacitance is.
    for (j = 0; j < 6; j++)
    {
        for (i = 0; i < initialAverageCount; i++)
        {
            accumulator += get_cap_level(j);
        }

        // Threshold is set to 2x average.
        threshold[j] = 2 * (accumulator / initialAverageCount);
    }
}

void updatePinState(void)
{
    unsigned int i;

    unsigned int j;
    for (i = 0; i < 6; i++)
    {
        pinState[i] = LOW;
        for (j = 0; j < sampleLoopSize; j++)
        {
            if (get_cap_level(i) > threshold[i])
            {
                pinState[i] = HIGH;
            }
        }
    }
}

// returns the lowest-number pin that is currently pressed
int whichPinPressed(void)
{
    unsigned int i;

    for (i = 0; i < 6; i++)
    {
        if (pinState[i] == HIGH)
            return i;
    }

    // if none changed, return -1
    return -1;
}

void loop(void)
{
    int whichPin;
    // function to read all pins and copy to an array
    updatePinState();

    whichPin = whichPinPressed();
    if ((whichPin != -1) && (pinFocus == -2))
    {
        // reset the debouncing timer
        lastDebounceTime = millis();
        pinFocus = whichPin;
    }

    if ((millis() - lastDebounceTime) > debounceDelay)
    {
        // if the current pin we're looking at is still the focus, then we've got a hit
        if (whichPin == pinFocus)
        {
            if (pinHeld == 0)
            {
                // press the key on the USB keyboard
                Keyboard.press(keys[pinFocus]);
                delay(10);
                pinHeld = 1;
                lastFocus = pinFocus;
            }
            else
            {
                // if pin is being held, do nothing until it's released...
            }
        }
        else
        {
            if (whichPin == -1)
            {
                Keyboard.releaseAll();
                pinFocus = -2; // we're not looking at /any/ pin, reset pinHeld state
                pinHeld = 0;
            }
            else
            {
                // in this case we rolled over to another pin...
                // release the last key pressed
                Keyboard.release(keys[lastFocus]);

                // restart the debounce code path
                pinHeld = 0;
                pinFocus = whichPin;
                lastDebounceTime = millis();
            }
        }
    }
}
