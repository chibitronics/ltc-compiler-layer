/*
 Servo.cpp - Interrupt driven Servo library for Arduino using 16 bit timers- Version 2
 Copyright (c) 2009 Michael Margolis.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* 
 
 A servo is activated by creating an instance of the Servo class passing the desired pin to the attach() method.
 The servos are pulsed in the background using the value most recently written using the write() method
 
 Note that analogWrite of PWM on pins associated with the timer are disabled when the first servo is attached.
 Timers are seized as needed in groups of 12 servos - 24 servos use two timers, 48 servos will use four.
 
 The methods are:
 
 Servo - Class for manipulating servo motors connected to Arduino pins.
 
 attach(pin )  - Attaches a servo motor to an i/o pin.
 attach(pin, min, max  ) - Attaches to a pin setting min and max values in microseconds
 default min is 544, max is 2400  
 
 write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
 writeMicroseconds() - Sets the servo pulse width in microseconds 
 read()      - Gets the last written servo pulse width as an angle between 0 and 180. 
 readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
 attached()  - Returns true if there is a servo attached. 
 detach()    - Stops an attached servos from pulsing its i/o pin. 
 
*/

#include "Arduino.h"
#include "ChibiOS.h"
#include "Servo.h"
#include "kl02.h"
#include "memio.h"

// Sourced from 4 MHz IRC via a /2 divider.
#define TIMER_RATE 2000000

// Defualt number of ticks for a 1.5 mS pulse, at 2.00 MHz, or 2000000 * 0.001500.
#define SERVO_DEFAULT_TICKS 3000

// Number of ticks in 20 ms, or 2000000 * 0.02.
#define SERVO_COMPLETE_TICKS 40000

// Maximum number of servos controlled by one timer.
#define SERVOS_PER_TIMER   6

// Compensation ticks to trim adjust for digitalWrite delays 
#define TRIM_DURATION     (SERVOS_PER_TIMER/2)

#define NBR_TIMERS        (MAX_SERVOS / SERVOS_PER_TIMER)

// static array of servo structures
static servo_t servos[MAX_SERVOS];

// counter for the servo being pulsed for each timer (or -1 if refresh interval)
static uint8_t current_channel;

// this is the sequence for timer utilization on other controllers 
typedef enum { _timer1 } servoTimer_t;

// the total number of attached servos
static uint8_t ServoCount = 0;

// The numver of servos that have been attach()ed
static uint8_t active_servo_count;

// convenience macros
#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max)  // maximum value in uS for this servo 

/* Decrement this for each servo we hit.  When it reaches 0, reset the cycle. */
static uint32_t ticks_remaining = SERVO_COMPLETE_TICKS;

#define NO_CHANNEL 0xff

/************ static functions common to all instances ***********************/

static int timerFastISR(void)
{
  // Either lower the previous pulse, or if there was no previous pulse then
  // reset the timer.
  if (current_channel != NO_CHANNEL) {
    if (current_channel < ServoCount && servos[current_channel].Pin.isActive == true)
      // Pulse this channel low if activated.
      digitalWrite(servos[current_channel].Pin.nbr, LOW);
  }
  else
    // current_channel indicated that refresh interval completed.
    // Reset the timer.
    ticks_remaining = SERVO_COMPLETE_TICKS;

  // Increment to the next channel.
  current_channel++;

  // Handle the next channel, or fill in the remaining gap.
  if ( (current_channel < ServoCount) && (current_channel < SERVOS_PER_TIMER) ) {

    uint32_t ticks = servos[current_channel].ticks * 2;
    writel(ticks, LPTMR0_CMR);
    ticks_remaining -= ticks;

    // Check if activated.
    if (servos[current_channel].Pin.isActive == true)
      // It's an active channel so pulse it high.
      digitalWrite(servos[current_channel].Pin.nbr, HIGH);
  }  
  else { 
    // Finished all channels.  Wait for the refresh period to expire
    // before starting over.
    writel(ticks_remaining, LPTMR0_CMR);

    // This will get incremented at the end of the refresh period
    // to start again at the first channel.
    current_channel = NO_CHANNEL;
  }

  // Clear the TCF bit, which lets the timer continue.
  writel(LPTMR_CSR_TCF | LPTMR_CSR_TIE | LPTMR_CSR_TEN, LPTMR0_CSR);

  return 0;
}

static void init_isr(void)
{
  // Don't re-init if things are already running
  if (active_servo_count)
    return;

  enableTimer(1);
  attachFastInterrupt(LPTMR_IRQ, timerFastISR);

  // Start the timer firing.
  ticks_remaining = SERVO_COMPLETE_TICKS;
  writel(ticks_remaining, LPTMR0_CMR);
  writel(LPTMR_CSR_TIE | LPTMR_CSR_TEN, LPTMR0_CSR);
}

static void deinit_isr(void)
{
  if (!active_servo_count)
    return;

  writel(0, LPTMR0_CSR);
  detachFastInterrupt(LPTMR_IRQ);
}

/****************** end of static functions ******************************/

Servo::Servo()
{
  if (ServoCount < MAX_SERVOS) {

    // Assign a servo index to this instance.
    this->servoIndex = ServoCount++;

    // Store default values.
    servos[this->servoIndex].ticks = SERVO_DEFAULT_TICKS;
  }
  else
    this->servoIndex = INVALID_SERVO;  // too many servos
}

uint8_t Servo::attach(int pin)
{
  return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
}

uint8_t Servo::attach(int pin, int min, int max)
{
  if (this->servoIndex >= MAX_SERVOS)
    return this->servoIndex;

  pinMode(pin, OUTPUT);

  // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
  this->min = min;
  this->max = max;

  init_isr();
  active_servo_count++;

  servos[this->servoIndex].Pin.isActive = true;
  servos[this->servoIndex].Pin.nbr = canonicalizePin(pin);

  return this->servoIndex;
}

void Servo::detach()
{
  if (this->servoIndex >= MAX_SERVOS)
    return;

  servos[this->servoIndex].Pin.isActive = false;

  active_servo_count--;
  deinit_isr();
}

void Servo::write(int value)
{
  // Treat values less than 544 as angles in degrees.
  // Valid values in microseconds are handled as microseconds.
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    if (value > 180)
      value = 180;
    value = map(value, 0, 180, this->min, this->max);
  }
  this->writeMicroseconds(value);
}

void Servo::writeMicroseconds(int value)
{
  // calculate and store the values for the given channel
  uint8_t channel = this->servoIndex;
  // ensure channel is valid
  if (channel < MAX_SERVOS)
  {
    // ensure pulse width is valid
    if (value < (int)min)
      value = min;
    else if (value > (int)max)
      value = max;

    // Convert the usec to tick count.  Split the divides to avoid overflow.
    uint64_t conversion = TIMER_RATE;
    conversion = conversion * value;
    conversion = conversion / 1000000;
    servos[channel].ticks = conversion;
  }
}

int Servo::read() // return the value as degrees
{
  return map(this->readMicroseconds()+1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

int Servo::readMicroseconds()
{
  uint64_t pulse_width;
  if (this->servoIndex != INVALID_SERVO)
    pulse_width = (servos[this->servoIndex].ticks * 10000000) / TIMER_RATE;
  else 
    pulse_width = 0;

  return pulse_width;   
}

bool Servo::attached()
{
  return servos[this->servoIndex].Pin.isActive;
}
