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

#define NUM_DIGITAL_PINS            32
#define NUM_ANALOG_INPUTS           8
#define analogInputToDigitalPin(p)  (p+24)

#define digitalPinHasPWM(p)         ((p) == 4 || (p) == 5 || (p) == 7 || (p) == 19)

#define SS  20
#define MOSI 21
#define MISO 22
#define SCK  23

#define SDA 9
#define SCL 8
#define LED_BUILTIN 23

#define A0 24
#define A1 25
#define A2 26
#define A3 27
#define A4 28
#define A5 29
#define A6 30
#define A7 31

#define analogPinToChannel(p)  ((p)>=24?(p)-24:(p))

#ifdef ARDUINO_MAIN

// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// ATMEL ATMEGA16/32
//
//                    +--\/--+
//       D16 |  PB0  1|      |40  PA0  | D24 | A0
//       D17 |  PB1  2|      |39  PA1  | D25 | A1
//       D18 |  PB2  3|      |38  PA2  | D26 | A2
// PWM | D19 |  PB3  4|      |37  PA3  | D27 | A3
//       D20 |  PB4  5|      |36  PA4  | D28 | A4
//       D21 |  PB5  6|      |35  PA5  | D29 | A5
//       D22 |  PB6  7|      |34  PA6  | D30 | A6
// LED | D23 |  PB7  8|      |33  PA7  | D31 | A7
//              RST  9|      |32  AREF
//              VCC 10|      |31  GND         
//              GND 11|      |30  AVCC      
//            XTAL2 12|      |29  PC7  | D15
//            XTAL1 13|      |28  PC6  | D14
// RXD | D0  |  PD0 14|      |27  PC5  | D13
// TXD | D1  |  PD1 15|      |26  PC4  | D12
//       D2  |  PD2 16|      |25  PC3  | D11
//       D3  |  PD3 17|      |24  PC2  | D10
// PWM | D4  |  PD4 18|      |23  PC1  | D9  | SDA
// PWM | D5  |  PD5 19|      |22  PC0  | D8  | SCL
//       D6  |  PD6 20|      |21  PD7  | D7  | PWM
//                    +------+
//

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PORT_D, PORT_D, PORT_D, PORT_D,
	PORT_D, PORT_D, PORT_D, PORT_D,
	PORT_C, PORT_C, PORT_C, PORT_C,
	PORT_C, PORT_C, PORT_C, PORT_C,
	PORT_B, PORT_B, PORT_B, PORT_B,
	PORT_B, PORT_B, PORT_B, PORT_B,
	PORT_A, PORT_A, PORT_A, PORT_A,
	PORT_A, PORT_A, PORT_A, PORT_A,
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
	_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	TIMER1B, TIMER1A, NOT_ON_TIMER, TIMER2,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, TIMER0A,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
	NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER, NOT_ON_TIMER,
};

#endif

#endif
