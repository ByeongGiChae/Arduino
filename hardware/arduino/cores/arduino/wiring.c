/*
 wiring.c - Partial implementation of the Wiring API for the ATmega8.
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

 $Id$

 edited by Weihong Guan to improve efficiency. August, 2012
 */

#include "wiring_private.h"

#if F_CPU == 16000000
#define US_PER_TIMER0_COUNT	4
#define SCALE_US_PER_TIMER0_COUNT 2
#define TIMER_COUNT_PER_MS	250
#elif F_CPU == 8000000
#define US_PER_TIMER0_COUNT	8
#define SCALE_US_PER_TIMER0_COUNT 3
#define TIMER_COUNT_PER_MS	125
#endif

volatile unsigned long timer0_overflow_count = 0;

// W.H. Guan: Do this long division just once. US means us unit, microsecond, 10 ^ (-6) second.
ISR(TIMER0_OVF_vect)
{
	timer0_overflow_count++;
}

// W.H. Guan: a timer0_overflow takes 1024 us, which is closed enough to 1000 us.
// 1 millis = 1024 us
unsigned long millis()
{
	return timer0_overflow_count;
}

inline unsigned long time_stamp()
{
	return (TCNT0 + (timer0_overflow_count << 8));
}

unsigned long micros()
{
	unsigned long m = time_stamp() << SCALE_US_PER_TIMER0_COUNT;
	return m;
}

void delay(unsigned long ms)
{
	uint16_t start = (uint16_t) time_stamp();

	while (ms)
	{
		if (((uint16_t) time_stamp() - start) >= TIMER_COUNT_PER_MS)
		{
			ms--;
			start += TIMER_COUNT_PER_MS;
		}
	}
}

/* Delay for the given number of microseconds.  Assumes a 8 or 16 MHz clock. */
void delayMicroseconds(unsigned int us)
{
	// calling avrlib's delay_us() function with low values (e.g. 1 or
	// 2 microseconds) gives delays longer than desired.
	//delay_us(us);
#if F_CPU >= 20000000L
	// for the 20 MHz clock on rare Arduino boards

	// for a one-microsecond delay, simply wait 2 cycle and return. The overhead
	// of the function call yields a delay of exactly a one microsecond.
	__asm__ __volatile__ (
			"nop" "\n\t"
			"nop");//just waiting 2 cycle
	if (--us == 0)
	return;

	// the following loop takes a 1/5 of a microsecond (4 cycles)
	// per iteration, so execute it five times for each microsecond of
	// delay requested.
	us = (us<<2) + us;// x5 us

	// account for the time taken in the preceeding commands.
	us -= 2;

#elif F_CPU >= 16000000L
	// for the 16 MHz clock on most Arduino boards

	// for a one-microsecond delay, simply return.  the overhead
	// of the function call yields a delay of approximately 1 1/8 us.
	if (--us == 0)
		return;

	// the following loop takes a quarter of a microsecond (4 cycles)
	// per iteration, so execute it four times for each microsecond of
	// delay requested.
	us <<= 2;

	// account for the time taken in the preceeding commands.
	us -= 2;
#else
	// for the 8 MHz internal clock on the ATmega168

	// for a one- or two-microsecond delay, simply return.  the overhead of
	// the function calls takes more than two microseconds.  can't just
	// subtract two, since us is unsigned; we'd overflow.
	if (--us == 0)
	return;
	if (--us == 0)
	return;

	// the following loop takes half of a microsecond (4 cycles)
	// per iteration, so execute it twice for each microsecond of
	// delay requested.
	us <<= 1;

	// partially compensate for the time taken by the preceeding commands.
	// we can't subtract any more than this or we'd overflow w/ small delays.
	us--;
#endif

	// busy wait
//	__asm__ __volatile__ (
//			"1: sbiw %0,1" "\n\t" // 2 cycles
//			"brne 1b" : "=w" (us) : "0" (us)// 2 cycles
//	);
	_delay_loop_2(us);
	// W.H. Guan: it is asm function is already defined in <util/delay_basic.h>
}

void init()
{
	// this needs to be called before setup() or some functions won't work there
	sei();

	#if defined(TCCR0)
		TIMER_CS(TCCR0, 0, TIMER_WITH_EXT_CLK_CS_64);
		#if defined(WGM00) && defined(WGM01)
		TIMER_2BIT_WAVEFORM(0, TIMER_2BIT_WAVEFORM_FPWM);
		#endif
	#elif defined(TCCR0A) && defined(TCCR0B)
		TIMER_CS(TCCR0B, 0, TIMER_WITH_EXT_CLK_CS_64);
		TIMER_3BIT_WAVEFORM(0, TIMER_3BIT_WAVEFORM_FPWM);
	#else
		#error Timer 0 reg not found
	#endif

	#if defined(TIMSK)
		sbi(TIMSK, TOIE0);
	#elif defined(TIMSK0)
		sbi(TIMSK0, TOIE0);
	#else
		#error	Timer 0 overflow interrupt not set correctly
	#endif

	TIMER_CS(TCCR1B, 1, TIMER_WITH_EXT_CLK_CS_64);
	TIMER_4BIT_WAVEFORM(1, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);

	#if defined(TCCR2)
		TIMER_CS(TCCR2, 2, TIMER_WITHOUT_EXT_CLK_CS_64);
		TIMER_2BIT_WAVEFORM(2, TIMER_2BIT_WAVEFORM_PCPWM);
	#elif defined(TCCR2A) && defined(TCCR2B)
		TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_64);
		TIMER_3BIT_WAVEFORM(2, TIMER_3BIT_WAVEFORM_PCPWM);
	#else
		#error Timer 2 reg not found
	#endif

	#if defined(TCCR3A) && defined(TCCR3B)
		TIMER_CS(TCCR3B, 3, TIMER_WITH_EXT_CLK_CS_64);
		TIMER_4BIT_WAVEFORM(3, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
	#endif

#if defined(TCCR4A) && defined(TCCR4B)
	#if defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
		sbi(TCCR4B, CS42); // set timer4 prescale factor to 64
		sbi(TCCR4B, CS41);
		sbi(TCCR4B, CS40);
		sbi(TCCR4D, WGM40);// put timer 4 in phase- and frequency-correct PWM mode
		sbi(TCCR4A, PWM4A);// enable PWM mode for comparator OCR4A
		sbi(TCCR4C, PWM4D);// enable PWM mode for comparator OCR4D
	#else
		TIMER_CS(TCCR4B, 4, TIMER_WITH_EXT_CLK_CS_64);
		TIMER_4BIT_WAVEFORM(4, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
	#endif
#endif

#if defined(TCCR5A) && defined(TCCR5B)
	TIMER_CS(TCCR5B, 5, TIMER_WITH_EXT_CLK_CS_64);
	TIMER_4BIT_WAVEFORM(5, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
#endif

#if defined(ADCSRA)
	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other clock speeds, and
	// this code should use F_CPU to determine the prescale factor.
	ADC_PRESCALER(ADC_PRECSCALER_DIVISION_128);
	// enable a2d conversions
	sbi(ADCSRA, ADEN);
#else
	#error ADC initial fail
#endif

	// the bootloader connects pins 0 and 1 to the USART; disconnect them
	// here so they can be used as normal digital i/o; they will be
	// reconnected in Serial.begin()
#if defined(UCSRB)
	UCSRB = 0;
#elif defined(UCSR0B)
	UCSR0B = 0;
#endif
}
