/*
 pins_arduino.h - Pin definition functions for Arduino
 Part of Arduino - http://www.arduino.cc/

 Copyright (c) 2007 David A. Mellis

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General
 Public License along with this library; if not, write to the
 Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 Boston, MA  02111-1307  USA

 $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
 */

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define NUM_DIGITAL_PINS            20
#define NUM_ANALOG_INPUTS           6
#define analogInputToDigitalPin(p)  ((p < 6) ? (p) + 14 : -1)

#define digitalPinHasPWM(p)         ((p) == 3 || (p) == 5 || (p) == 6 || (p) == 9 || (p) == 10 || (p) == 11)

static const uint8_t SS = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK = 13;

static const uint8_t SDA = 18;
static const uint8_t SCL = 19;
static const uint8_t LED_BUILTIN = 13;

static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;
static const uint8_t A6 = 20;
static const uint8_t A7 = 21;

#define analogPinToChannel(p)  ((p)>=14?(p)-14:(p))

#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA 168 / 328
//
//                     +-\/-+
//        RST |  PC6  1|    |28  PC5  | D19 | A5  | SCL
//  RXD | D0  |  PD0  2|    |27  PC4  | D18 | A4  | SDA
//  TXD | D1  |  PD1  3|    |26  PC3  | D17 | A3
//        D2  |  PD2  4|    |25  PC2  | D16 | A2
//  PWM | D3  |  PD3  5|    |24  PC1  | D15 | A1
//        D4  |  PD4  6|    |23  PC0  | D14 | A0
//               VCC  7|    |22  GND
//               GND  8|    |21  AREF
//             XTAL2  9|    |20  AVCC
//             XTAL1 10|    |19  PB5  | D13 | LED | SCK
//  PWM | D5  |  PD5 11|    |18  PB4  | D12       | MISO
//  PWM | D6  |  PD6 12|    |17  PB3  | D11 | PWM | MOSI 
//        D7  |  PD7 13|    |16  PB2  | D10 | PWM | SS
//        D8  |  PB0 14|    |15  PB1  | D9  | PWM
//                     +----+
//
// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PORT,
	NOT_A_PORT,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	PORT_D, PORT_D, PORT_D, PORT_D, PORT_D, PORT_D, PORT_D, PORT_D,
	PORT_B, PORT_B, PORT_B, PORT_B, PORT_B, PORT_B,
	PORT_C, PORT_C, PORT_C, PORT_C, PORT_C, PORT_C,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), _BV(0),
	_BV(1), _BV(2), _BV(3), _BV(4), _BV(5),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, TIMER2B,
	NOT_ON_TIMER, TIMER0B, TIMER0A, NOT_ON_TIMER,
	NOT_ON_TIMER, TIMER1A, TIMER1B, TIMER2A,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
};

#endif

#endif
