/*
 wiring_private.h - Internal header file.
 Part of Arduino - http://www.arduino.cc/

 Copyright (c) 2005-2006 David A. Mellis

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

 $Id: wiring.h 239 2007-01-12 17:58:39Z mellis $
 */

#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdarg.h>
#include <util/delay_basic.h>
#include "Arduino.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef dbi
#define dbi(sfr, bit, val) (val)?sbi(sfr, bit):cbi(sfr, bit)
#endif

#define EXTERNAL_INT_0 0
#define EXTERNAL_INT_1 1
#define EXTERNAL_INT_2 2
#define EXTERNAL_INT_3 3
#define EXTERNAL_INT_4 4
#define EXTERNAL_INT_5 5
#define EXTERNAL_INT_6 6
#define EXTERNAL_INT_7 7

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define EXTERNAL_NUM_INTERRUPTS 8
#elif defined(__AVR_ATmega1284P__) 
#define EXTERNAL_NUM_INTERRUPTS 3
#elif defined(__AVR_ATmega32U4__)
#define EXTERNAL_NUM_INTERRUPTS 4
#else
#define EXTERNAL_NUM_INTERRUPTS 2
#endif

#define TIMER_WITH_EXT_CLK_CS_NUL	0
#define TIMER_WITH_EXT_CLK_CS_1		1
#define TIMER_WITH_EXT_CLK_CS_8		2
#define TIMER_WITH_EXT_CLK_CS_64	3
#define TIMER_WITH_EXT_CLK_CS_256	4
#define TIMER_WITH_EXT_CLK_CS_1024	5
#define TIMER_WITH_EXT_CLK_CS_EXT_FALL	6
#define TIMER_WITH_EXT_CLK_CS_EXT_RISE	7

#define TIMER_WITHOUT_EXT_CLK_CS_NUL	0
#define TIMER_WITHOUT_EXT_CLK_CS_1		1
#define TIMER_WITHOUT_EXT_CLK_CS_8		2
#define TIMER_WITHOUT_EXT_CLK_CS_32		3
#define TIMER_WITHOUT_EXT_CLK_CS_64		4
#define TIMER_WITHOUT_EXT_CLK_CS_128	5
#define TIMER_WITHOUT_EXT_CLK_CS_256	6
#define TIMER_WITHOUT_EXT_CLK_CS_1024	7

#define TIMER_4BIT_WAVEFROM_NORMAL			0
#define TIMER_4BIT_WAVEFROM_PCPWM_8BIT		1
#define TIMER_4BIT_WAVEFROM_PCPWM_9BIT		2
#define TIMER_4BIT_WAVEFROM_PCPWM_10BIT		3
#define TIMER_4BIT_WAVEFROM_CTC_OCR1A		4
#define TIMER_4BIT_WAVEFROM_FASTPWM_8BIT	5
#define TIMER_4BIT_WAVEFROM_FASTPWM_9BIT	6
#define TIMER_4BIT_WAVEFROM_FASTPWM_10BIT	7
#define TIMER_4BIT_WAVEFROM_PFCPWM_ICR1		8
#define TIMER_4BIT_WAVEFROM_PFCPWM_OCR1A	9
#define TIMER_4BIT_WAVEFROM_PCPWM_ICR1		10
#define TIMER_4BIT_WAVEFROM_PCPWM_OCR1A		11
#define TIMER_4BIT_WAVEFROM_CTC_ICR1		12
#define TIMER_4BIT_WAVEFROM_FASTPWM_ICR1	14
#define TIMER_4BIT_WAVEFROM_FASTPWM__OCR1A	15

#define TIMER_3BIT_WAVEFORM_NORMAL	0
#define TIMER_3BIT_WAVEFORM_PCPWM	1
#define TIMER_3BIT_WAVEFORM_CTC		2
#define TIMER_3BIT_WAVEFORM_FPWM	3
#define TIMER_3BIT_WAVEFORM_PCPWM_OCRA	5
#define TIMER_3BIT_WAVEFORM_FPWM_OCRA	7

#define TIMER_2BIT_WAVEFORM_NORMAL	0
#define TIMER_2BIT_WAVEFORM_PCPWM	1
#define TIMER_2BIT_WAVEFORM_CTC		2
#define TIMER_2BIT_WAVEFORM_FPWM	3

#define ADC_PRECSCALER_DIVISION_2	0
#define ADC_PRECSCALER_DIVISION_4	2
#define ADC_PRECSCALER_DIVISION_8	3
#define ADC_PRECSCALER_DIVISION_16	4
#define ADC_PRECSCALER_DIVISION_32	5
#define ADC_PRECSCALER_DIVISION_64	6
#define ADC_PRECSCALER_DIVISION_128	7

#define TIMER_CS(reg, n, clock_selection)	\
	dbi(reg, CS##n##2, bitRead(clock_selection, 2));	\
	dbi(reg, CS##n##1, bitRead(clock_selection, 1));	\
	dbi(reg, CS##n##0, bitRead(clock_selection, 0))

#define TIMER_2BIT_WAVEFORM(n, waveform)	\
	dbi(TCCR##n, WGM##n##1, bitRead(waveform, 1));	\
	dbi(TCCR##n, WGM##n##0, bitRead(waveform, 0))

#define TIMER_3BIT_WAVEFORM(n, waveform)	\
	dbi(TCCR##n##B, WGM##n##2, bitRead(waveform, 2));	\
	dbi(TCCR##n##A, WGM##n##1, bitRead(waveform, 1));	\
	dbi(TCCR##n##A, WGM##n##0, bitRead(waveform, 0))

#define TIMER_4BIT_WAVEFORM(n, waveform)	\
	dbi(TCCR##n##B, WGM##n##3, bitRead(waveform, 3));	\
	dbi(TCCR##n##B, WGM##n##2, bitRead(waveform, 2));	\
	dbi(TCCR##n##A, WGM##n##1, bitRead(waveform, 1));	\
	dbi(TCCR##n##A, WGM##n##0, bitRead(waveform, 0))

#define ADC_PRESCALER(division)	\
	dbi(ADCSRA, ADPS2, bitRead(division, 2));	\
	dbi(ADCSRA, ADPS1, bitRead(division, 1));	\
	dbi(ADCSRA, ADPS0, bitRead(division, 0))

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
