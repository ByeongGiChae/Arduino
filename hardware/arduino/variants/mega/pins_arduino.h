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

#define NUM_DIGITAL_PINS            70
#define NUM_ANALOG_INPUTS           16
#define analogInputToDigitalPin(p)  ((p < 16) ? (p) + 54 : -1)
#define digitalPinHasPWM(p)         (((p) >= 2 && (p) <= 13) || ((p) >= 44 && (p)<= 46))

static const uint8_t SS = 53;
static const uint8_t MOSI = 51;
static const uint8_t MISO = 50;
static const uint8_t SCK = 52;

static const uint8_t SDA = 20;
static const uint8_t SCL = 21;
static const uint8_t LED_BUILTIN = 13;

static const uint8_t A0 = 54;
static const uint8_t A1 = 55;
static const uint8_t A2 = 56;
static const uint8_t A3 = 57;
static const uint8_t A4 = 58;
static const uint8_t A5 = 59;
static const uint8_t A6 = 60;
static const uint8_t A7 = 61;
static const uint8_t A8 = 62;
static const uint8_t A9 = 63;
static const uint8_t A10 = 64;
static const uint8_t A11 = 65;
static const uint8_t A12 = 66;
static const uint8_t A13 = 67;
static const uint8_t A14 = 68;
static const uint8_t A15 = 69;

#define analogPinToChannel(P)  ((P)-54)

// A majority of the pins are NOT PCINTs, SO BE WARNED (i.e. you cannot use them as receive pins)
// Only pins available for RECEIVE (TRANSMIT can be on any pin):
// (I've deliberately left out pin mapping to the Hardware USARTs - seems senseless to me)
// Pins: 10, 11, 12, 13,  50, 51, 52, 53,  62, 63, 64, 65, 66, 67, 68, 69

#define digitalPinToPCICR(p)    ( (((p) >= 10) && ((p) <= 13)) || \
                                  (((p) >= 50) && ((p) <= 53)) || \
                                  (((p) >= 62) && ((p) <= 69)) ? (&PCICR) : ((uint8_t *)0) )

#define digitalPinToPCICRbit(p) ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? 0 : \
                                ( (((p) >= 62) && ((p) <= 69)) ? 2 : \
                                0 ) )

#define digitalPinToPCMSK(p)    ( (((p) >= 10) && ((p) <= 13)) || (((p) >= 50) && ((p) <= 53)) ? (&PCMSK0) : \
                                ( (((p) >= 62) && ((p) <= 69)) ? (&PCMSK2) : \
                                ((uint8_t *)0) ) )

#define digitalPinToPCMSKbit(p) ( (((p) >= 10) && ((p) <= 13)) ? ((p) - 6) : \
                                ( ((p) == 50) ? 3 : \
                                ( ((p) == 51) ? 2 : \
                                ( ((p) == 52) ? 1 : \
                                ( ((p) == 53) ? 0 : \
                                ( (((p) >= 62) && ((p) <= 69)) ? ((p) - 62) : \
                                0 ) ) ) ) ) )

#ifdef ARDUINO_MAIN

const uint16_t PROGMEM port_to_mode_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
	(uint16_t) &DDRE,
	(uint16_t) &DDRF,
	(uint16_t) &DDRG,
	(uint16_t) &DDRH,
	NOT_A_PORT,
	(uint16_t) &DDRJ,
	(uint16_t) &DDRK,
	(uint16_t) &DDRL,
};

const uint16_t PROGMEM port_to_output_PGM[] =
{
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
	(uint16_t) &PORTE,
	(uint16_t) &PORTF,
	(uint16_t) &PORTG,
	(uint16_t) &PORTH,
	NOT_A_PORT,
	(uint16_t) &PORTJ,
	(uint16_t) &PORTK,
	(uint16_t) &PORTL,
};

const uint16_t PROGMEM port_to_input_PGM[] =
{
	NOT_A_PIN,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
	(uint16_t) &PINE,
	(uint16_t) &PINF,
	(uint16_t) &PING,
	(uint16_t) &PINH,
	NOT_A_PIN,
	(uint16_t) &PINJ,
	(uint16_t) &PINK,
	(uint16_t) &PINL,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] =
{
	// PORTLIST		
	// -------------------------------------------		
	PORT_E ,// PORT_E 0 ** 0 ** USART0_RX
	PORT_E ,// PORT_E 1 ** 1 ** USART0_TX
	PORT_E ,// PORT_E 4 ** 2 ** PORT_WM2
	PORT_E ,// PORT_E 5 ** 3 ** PORT_WM3
	PORT_G ,// PORT_G 5 ** 4 ** PORT_WM4
	PORT_E ,// PORT_E 3 ** 5 ** PORT_WM5
	PORT_H ,// PORT_H 3 ** 6 ** PORT_WM6
	PORT_H ,// PORT_H 4 ** 7 ** PORT_WM7
	PORT_H ,// PORT_H 5 ** 8 ** PORT_WM8
	PORT_H ,// PORT_H 6 ** 9 ** PORT_WM9
	PORT_B ,// PORT_B 4 ** 10 ** PORT_WM10
	PORT_B ,// PORT_B 5 ** 11 ** PORT_WM11
	PORT_B ,// PORT_B 6 ** 12 ** PORT_WM12
	PORT_B ,// PORT_B 7 ** 13 ** PORT_WM13
	PORT_J ,// PORT_J 1 ** 14 ** USART3_TX
	PORT_J ,// PORT_J 0 ** 15 ** USART3_RX
	PORT_H ,// PORT_H 1 ** 16 ** USART2_TX
	PORT_H ,// PORT_H 0 ** 17 ** USART2_RX
	PORT_D ,// PORT_D 3 ** 18 ** USART1_TX
	PORT_D ,// PORT_D 2 ** 19 ** USART1_RX
	PORT_D ,// PORT_D 1 ** 20 ** I2C_SDA
	PORT_D ,// PORT_D 0 ** 21 ** I2C_SCL
	PORT_A ,// PORT_A 0 ** 22 ** D22
	PORT_A ,// PORT_A 1 ** 23 ** D23
	PORT_A ,// PORT_A 2 ** 24 ** D24
	PORT_A ,// PORT_A 3 ** 25 ** D25
	PORT_A ,// PORT_A 4 ** 26 ** D26
	PORT_A ,// PORT_A 5 ** 27 ** D27
	PORT_A ,// PORT_A 6 ** 28 ** D28
	PORT_A ,// PORT_A 7 ** 29 ** D29
	PORT_C ,// PORT_C 7 ** 30 ** D30
	PORT_C ,// PORT_C 6 ** 31 ** D31
	PORT_C ,// PORT_C 5 ** 32 ** D32
	PORT_C ,// PORT_C 4 ** 33 ** D33
	PORT_C ,// PORT_C 3 ** 34 ** D34
	PORT_C ,// PORT_C 2 ** 35 ** D35
	PORT_C ,// PORT_C 1 ** 36 ** D36
	PORT_C ,// PORT_C 0 ** 37 ** D37
	PORT_D ,// PORT_D 7 ** 38 ** D38
	PORT_G ,// PORT_G 2 ** 39 ** D39
	PORT_G ,// PORT_G 1 ** 40 ** D40
	PORT_G ,// PORT_G 0 ** 41 ** D41
	PORT_L ,// PORT_L 7 ** 42 ** D42
	PORT_L ,// PORT_L 6 ** 43 ** D43
	PORT_L ,// PORT_L 5 ** 44 ** D44
	PORT_L ,// PORT_L 4 ** 45 ** D45
	PORT_L ,// PORT_L 3 ** 46 ** D46
	PORT_L ,// PORT_L 2 ** 47 ** D47
	PORT_L ,// PORT_L 1 ** 48 ** D48
	PORT_L ,// PORT_L 0 ** 49 ** D49
	PORT_B ,// PORT_B 3 ** 50 ** SPI_MISO
	PORT_B ,// PORT_B 2 ** 51 ** SPI_MOSI
	PORT_B ,// PORT_B 1 ** 52 ** SPI_SCK
	PORT_B ,// PORT_B 0 ** 53 ** SPI_SS
	PORT_F ,// PORT_F 0 ** 54 ** A0
	PORT_F ,// PORT_F 1 ** 55 ** A1
	PORT_F ,// PORT_F 2 ** 56 ** A2
	PORT_F ,// PORT_F 3 ** 57 ** A3
	PORT_F ,// PORT_F 4 ** 58 ** A4
	PORT_F ,// PORT_F 5 ** 59 ** A5
	PORT_F ,// PORT_F 6 ** 60 ** A6
	PORT_F ,// PORT_F 7 ** 61 ** A7
	PORT_K ,// PORT_K 0 ** 62 ** A8
	PORT_K ,// PORT_K 1 ** 63 ** A9
	PORT_K ,// PORT_K 2 ** 64 ** A10
	PORT_K ,// PORT_K 3 ** 65 ** A11
	PORT_K ,// PORT_K 4 ** 66 ** A12
	PORT_K ,// PORT_K 5 ** 67 ** A13
	PORT_K ,// PORT_K 6 ** 68 ** A14
	PORT_K ,// PORT_K 7 ** 69 ** A15
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] =
{
	// PIN IN PORT		
	// -------------------------------------------		
	_BV( 0 ) ,// PE 0 ** 0 ** USART0_RX
	_BV( 1 ) ,// PE 1 ** 1 ** USART0_TX
	_BV( 4 ) ,// PE 4 ** 2 ** PWM2
	_BV( 5 ) ,// PE 5 ** 3 ** PWM3
	_BV( 5 ) ,// PG 5 ** 4 ** PWM4
	_BV( 3 ) ,// PE 3 ** 5 ** PWM5
	_BV( 3 ) ,// PH 3 ** 6 ** PWM6
	_BV( 4 ) ,// PH 4 ** 7 ** PWM7
	_BV( 5 ) ,// PH 5 ** 8 ** PWM8
	_BV( 6 ) ,// PH 6 ** 9 ** PWM9
	_BV( 4 ) ,// PB 4 ** 10 ** PWM10
	_BV( 5 ) ,// PB 5 ** 11 ** PWM11
	_BV( 6 ) ,// PB 6 ** 12 ** PWM12
	_BV( 7 ) ,// PB 7 ** 13 ** PWM13
	_BV( 1 ) ,// PJ 1 ** 14 ** USART3_TX
	_BV( 0 ) ,// PJ 0 ** 15 ** USART3_RX
	_BV( 1 ) ,// PH 1 ** 16 ** USART2_TX
	_BV( 0 ) ,// PH 0 ** 17 ** USART2_RX
	_BV( 3 ) ,// PD 3 ** 18 ** USART1_TX
	_BV( 2 ) ,// PD 2 ** 19 ** USART1_RX
	_BV( 1 ) ,// PD 1 ** 20 ** I2C_SDA
	_BV( 0 ) ,// PD 0 ** 21 ** I2C_SCL
	_BV( 0 ) ,// PA 0 ** 22 ** D22
	_BV( 1 ) ,// PA 1 ** 23 ** D23
	_BV( 2 ) ,// PA 2 ** 24 ** D24
	_BV( 3 ) ,// PA 3 ** 25 ** D25
	_BV( 4 ) ,// PA 4 ** 26 ** D26
	_BV( 5 ) ,// PA 5 ** 27 ** D27
	_BV( 6 ) ,// PA 6 ** 28 ** D28
	_BV( 7 ) ,// PA 7 ** 29 ** D29
	_BV( 7 ) ,// PC 7 ** 30 ** D30
	_BV( 6 ) ,// PC 6 ** 31 ** D31
	_BV( 5 ) ,// PC 5 ** 32 ** D32
	_BV( 4 ) ,// PC 4 ** 33 ** D33
	_BV( 3 ) ,// PC 3 ** 34 ** D34
	_BV( 2 ) ,// PC 2 ** 35 ** D35
	_BV( 1 ) ,// PC 1 ** 36 ** D36
	_BV( 0 ) ,// PC 0 ** 37 ** D37
	_BV( 7 ) ,// PD 7 ** 38 ** D38
	_BV( 2 ) ,// PG 2 ** 39 ** D39
	_BV( 1 ) ,// PG 1 ** 40 ** D40
	_BV( 0 ) ,// PG 0 ** 41 ** D41
	_BV( 7 ) ,// PL 7 ** 42 ** D42
	_BV( 6 ) ,// PL 6 ** 43 ** D43
	_BV( 5 ) ,// PL 5 ** 44 ** D44
	_BV( 4 ) ,// PL 4 ** 45 ** D45
	_BV( 3 ) ,// PL 3 ** 46 ** D46
	_BV( 2 ) ,// PL 2 ** 47 ** D47
	_BV( 1 ) ,// PL 1 ** 48 ** D48
	_BV( 0 ) ,// PL 0 ** 49 ** D49
	_BV( 3 ) ,// PB 3 ** 50 ** SPI_MISO
	_BV( 2 ) ,// PB 2 ** 51 ** SPI_MOSI
	_BV( 1 ) ,// PB 1 ** 52 ** SPI_SCK
	_BV( 0 ) ,// PB 0 ** 53 ** SPI_SS
	_BV( 0 ) ,// PF 0 ** 54 ** A0
	_BV( 1 ) ,// PF 1 ** 55 ** A1
	_BV( 2 ) ,// PF 2 ** 56 ** A2
	_BV( 3 ) ,// PF 3 ** 57 ** A3
	_BV( 4 ) ,// PF 4 ** 58 ** A4
	_BV( 5 ) ,// PF 5 ** 59 ** A5
	_BV( 6 ) ,// PF 6 ** 60 ** A6
	_BV( 7 ) ,// PF 7 ** 61 ** A7
	_BV( 0 ) ,// PK 0 ** 62 ** A8
	_BV( 1 ) ,// PK 1 ** 63 ** A9
	_BV( 2 ) ,// PK 2 ** 64 ** A10
	_BV( 3 ) ,// PK 3 ** 65 ** A11
	_BV( 4 ) ,// PK 4 ** 66 ** A12
	_BV( 5 ) ,// PK 5 ** 67 ** A13
	_BV( 6 ) ,// PK 6 ** 68 ** A14
	_BV( 7 ) ,// PK 7 ** 69 ** A15
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] =
{
	// TIMERS		
	// -------------------------------------------		
	NOT_ON_TIMER ,// PE 0 ** 0 ** USART0_RX
	NOT_ON_TIMER ,// PE 1 ** 1 ** USART0_TX
	TIMER3B ,// PE 4 ** 2 ** PWM2
	TIMER3C ,// PE 5 ** 3 ** PWM3
	TIMER0B ,// PG 5 ** 4 ** PWM4
	TIMER3A ,// PE 3 ** 5 ** PWM5
	TIMER4A ,// PH 3 ** 6 ** PWM6
	TIMER4B ,// PH 4 ** 7 ** PWM7
	TIMER4C ,// PH 5 ** 8 ** PWM8
	TIMER2B ,// PH 6 ** 9 ** PWM9
	TIMER2A ,// PB 4 ** 10 ** PWM10
	TIMER1A ,// PB 5 ** 11 ** PWM11
	TIMER1B ,// PB 6 ** 12 ** PWM12
	TIMER0A ,// PB 7 ** 13 ** PWM13
	NOT_ON_TIMER ,// PJ 1 ** 14 ** USART3_TX
	NOT_ON_TIMER ,// PJ 0 ** 15 ** USART3_RX
	NOT_ON_TIMER ,// PH 1 ** 16 ** USART2_TX
	NOT_ON_TIMER ,// PH 0 ** 17 ** USART2_RX
	NOT_ON_TIMER ,// PD 3 ** 18 ** USART1_TX
	NOT_ON_TIMER ,// PD 2 ** 19 ** USART1_RX
	NOT_ON_TIMER ,// PD 1 ** 20 ** I2C_SDA
	NOT_ON_TIMER ,// PD 0 ** 21 ** I2C_SCL
	NOT_ON_TIMER ,// PA 0 ** 22 ** D22
	NOT_ON_TIMER ,// PA 1 ** 23 ** D23
	NOT_ON_TIMER ,// PA 2 ** 24 ** D24
	NOT_ON_TIMER ,// PA 3 ** 25 ** D25
	NOT_ON_TIMER ,// PA 4 ** 26 ** D26
	NOT_ON_TIMER ,// PA 5 ** 27 ** D27
	NOT_ON_TIMER ,// PA 6 ** 28 ** D28
	NOT_ON_TIMER ,// PA 7 ** 29 ** D29
	NOT_ON_TIMER ,// PC 7 ** 30 ** D30
	NOT_ON_TIMER ,// PC 6 ** 31 ** D31
	NOT_ON_TIMER ,// PC 5 ** 32 ** D32
	NOT_ON_TIMER ,// PC 4 ** 33 ** D33
	NOT_ON_TIMER ,// PC 3 ** 34 ** D34
	NOT_ON_TIMER ,// PC 2 ** 35 ** D35
	NOT_ON_TIMER ,// PC 1 ** 36 ** D36
	NOT_ON_TIMER ,// PC 0 ** 37 ** D37
	NOT_ON_TIMER ,// PD 7 ** 38 ** D38
	NOT_ON_TIMER ,// PG 2 ** 39 ** D39
	NOT_ON_TIMER ,// PG 1 ** 40 ** D40
	NOT_ON_TIMER ,// PG 0 ** 41 ** D41
	NOT_ON_TIMER ,// PL 7 ** 42 ** D42
	NOT_ON_TIMER ,// PL 6 ** 43 ** D43
	TIMER5C ,// PL 5 ** 44 ** D44
	TIMER5B ,// PL 4 ** 45 ** D45
	TIMER5A ,// PL 3 ** 46 ** D46
	NOT_ON_TIMER ,// PL 2 ** 47 ** D47
	NOT_ON_TIMER ,// PL 1 ** 48 ** D48
	NOT_ON_TIMER ,// PL 0 ** 49 ** D49
	NOT_ON_TIMER ,// PB 3 ** 50 ** SPI_MISO
	NOT_ON_TIMER ,// PB 2 ** 51 ** SPI_MOSI
	NOT_ON_TIMER ,// PB 1 ** 52 ** SPI_SCK
	NOT_ON_TIMER ,// PB 0 ** 53 ** SPI_SS
	NOT_ON_TIMER ,// PF 0 ** 54 ** A0
	NOT_ON_TIMER ,// PF 1 ** 55 ** A1
	NOT_ON_TIMER ,// PF 2 ** 56 ** A2
	NOT_ON_TIMER ,// PF 3 ** 57 ** A3
	NOT_ON_TIMER ,// PF 4 ** 58 ** A4
	NOT_ON_TIMER ,// PF 5 ** 59 ** A5
	NOT_ON_TIMER ,// PF 6 ** 60 ** A6
	NOT_ON_TIMER ,// PF 7 ** 61 ** A7
	NOT_ON_TIMER ,// PK 0 ** 62 ** A8
	NOT_ON_TIMER ,// PK 1 ** 63 ** A9
	NOT_ON_TIMER ,// PK 2 ** 64 ** A10
	NOT_ON_TIMER ,// PK 3 ** 65 ** A11
	NOT_ON_TIMER ,// PK 4 ** 66 ** A12
	NOT_ON_TIMER ,// PK 5 ** 67 ** A13
	NOT_ON_TIMER ,// PK 6 ** 68 ** A14
	NOT_ON_TIMER ,// PK 7 ** 69 ** A15
};

#endif

#endif
