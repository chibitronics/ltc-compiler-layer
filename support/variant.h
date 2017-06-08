/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _VARIANT_CHIBITRONICS_ESPLANADE_CODE_
#define _VARIANT_CHIBITRONICS_ESPLANADE_CODE_

/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/** Name of the board */
#define VARIANT_NAME "Love to Code"

/** Frequency of the board main oscillator */
#define VARIANT_MAINOSC		47972352UL  /* 32.768 kHz * 1464 (~48 MHz) */

/** Master clock frequency */
#define VARIANT_MCK			47972352UL  /* 32.768 kHz * 1464 (~48 MHz) */

/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "Arduino.h"
#include "LTC.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

/*----------------------------------------------------------------------------
 *        Pins
 *----------------------------------------------------------------------------*/

// Number of pins defined in PinDescription array
#define PINS_COUNT           (6u)
#define NUM_DIGITAL_PINS     (6u)
#define NUM_ANALOG_INPUTS    (6u)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 1

#define ADC_RESOLUTION		12

/*
 * PWM
 */
#define PWM_INTERFACE		PWM
#define PWM_INTERFACE_ID	ID_PWM
#define PWM_FREQUENCY		490
#define PWM_MAX_DUTY_CYCLE	255
#define PWM_MIN_DUTY_CYCLE	0
#define PWM_RESOLUTION		8

#ifdef __cplusplus
}
#endif

#endif /* _VARIANT_CHIBITRONICS_ESPLANADE_CODE_ */

