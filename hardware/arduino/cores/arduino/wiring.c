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
#define SCALE_US_PER_TIMER0_COUNT 2
#elif F_CPU == 8000000
#define SCALE_US_PER_TIMER0_COUNT 3
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
#define TIMER_4BIT_WAVEFROM_PCPWM_10BIT	3
#define TIMER_4BIT_WAVEFROM_CTC_OCR1A		4
#define TIMER_4BIT_WAVEFROM_FASTPWM_8BIT	5
#define TIMER_4BIT_WAVEFROM_FASTPWM_9BIT	6
#define TIMER_4BIT_WAVEFROM_FASTPWM_10BIT	7
#define TIMER_4BIT_WAVEFROM_PFCPWM_ICR1	8
#define TIMER_4BIT_WAVEFROM_PFCPWM_OCR1A	9
#define TIMER_4BIT_WAVEFROM_PCPWM_ICR1		10
#define TIMER_4BIT_WAVEFROM_PCPWM_OCR1A	11
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


volatile unsigned long timer0_overflow_count = 0;

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

// W.H. Guan: Do this long division just once. US means us unit, microsecond, 10 ^ (-6) second.
SIGNAL(TIMER0_OVF_vect)
{
	timer0_overflow_count++;
}

// W.H. Guan: a timer0_overflow takes 1024 us, which is closed enough to 1000 us.
// 1 millis = 1024 us
unsigned long millis()
{
	return timer0_overflow_count;
}

unsigned long micros()
{
	unsigned long m = (TCNT0 + (timer0_overflow_count << 8)) << SCALE_US_PER_TIMER0_COUNT;
	return m;
}

void delay(unsigned long ms)
{
	uint16_t start = (uint16_t) micros();

	while (ms)
	{
		if (((uint16_t) micros() - start) >= 1000)
		{
			ms--;
			start += 1000;
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

	// init Timer0, prescale = 64, waveform = fastpwm, enable overflow interruption
	{
		#if defined(CS02) && defined(CS01) && defined(CS00)

			#if defined(TCCR0)
				TIMER_CS(TCCR0, 0, TIMER_WITH_EXT_CLK_CS_64);
			#elif defined(TCCR0B)
				TIMER_CS(TCCR0B, 0, TIMER_WITH_EXT_CLK_CS_64);
			#else
				#error Timer 0 reg not found
			#endif

		#else
			#error Timer 0 prescale factor not set correctly
		#endif

		#if defined(TCCR0) && defined(WGM01) && defined(WGM00)
			TIMER_2BIT_WAVEFORM(0, TIMER_2BIT_WAVEFORM_FPWM);
		#elif defined(TCCR0A) && defined(WGM02) && defined(WGM01) && defined(WGM00)
			TIMER_3BIT_WAVEFORM(0, TIMER_3BIT_WAVEFORM_FPWM);
		#endif

		#if defined(TIMSK) && defined(TOIE0)
			sbi(TIMSK, TOIE0);
		#elif defined(TIMSK0) && defined(TOIE0)
			sbi(TIMSK0, TOIE0);
		#else
			#error	Timer 0 overflow interrupt not set correctly
		#endif
	}

	// init Timer1, prescale = 64, waveform = phase correct pwm
	{
		#if defined(TCCR1B) && defined(CS12) && defined(CS11) && defined(CS10)
			TIMER_CS(TCCR1B, 1, TIMER_WITH_EXT_CLK_CS_64);
		#else
			#error Timer 1 prescale factor not set correctly
		#endif

		#if defined(TCCR1A) && defined(TCCR1B) && defined(WGM13) && defined(WGM12) && defined(WGM11) && defined(WGM10)
			TIMER_4BIT_WAVEFORM(1, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
		#endif
	}

	// init Timer2, prescale = 64, waveform = phase correct pwm
	{
		#if defined(CS22) && defined(CS21) && defined(CS20)
			#if defined(TCCR2)
				TIMER_CS(TCCR2, 2, TIMER_WITHOUT_EXT_CLK_CS_64);
			#elif defined(TCCR2B)
				TIMER_CS(TCCR2B, 2, TIMER_WITHOUT_EXT_CLK_CS_64);
			#else
				#error Timer 2 reg not found
			#endif
		#else
			#error Timer 2 prescale factor not set correctly
		#endif

		#if defined(TCCR2) && defined(WGM21)  && defined(WGM20)
			TIMER_2BIT_WAVEFORM(2, TIMER_2BIT_WAVEFORM_PCPWM);
		#elif defined(TCCR2B) && defined(TCCR2A) && defined(WGM22) && defined(WGM21) && defined(WGM20)
			TIMER_3BIT_WAVEFORM(2, TIMER_3BIT_WAVEFORM_PCPWM);
		#endif
	}

	// init Timer3, presclae = 64, waveform = phase correct pwm
	{
		#if defined(TCCR3B) && defined(CS32) && defined(CS31) && defined(CS30)
			TIMER_CS(TCCR3B, 3, TIMER_WITH_EXT_CLK_CS_64)
		#endif

		#if defined(TCCR3B) && defined(TCCR3A) && defined(WGM33) && defined(WGM32) && defined(WGM31) && defined(WGM30)
			TIMER_4BIT_WAVEFORM(3, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
		#endif
	}

#if defined(TCCR4A) && defined(TCCR4B) && defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
	sbi(TCCR4B, CS42); // set timer4 prescale factor to 64
	sbi(TCCR4B, CS41);
	sbi(TCCR4B, CS40);
	sbi(TCCR4D, WGM40);// put timer 4 in phase- and frequency-correct PWM mode
	sbi(TCCR4A, PWM4A);// enable PWM mode for comparator OCR4A
	sbi(TCCR4C, PWM4D);// enable PWM mode for comparator OCR4D
#else
	// init Timer4, presclae = 64, waveform = phase correct pwm
	{
		#if defined(TCCR4B) && defined(CS42) && defined(CS41) && defined(CS40)
			TIMER_CS(TCCR4B, 4, TIMER_WITH_EXT_CLK_CS_64)
		#endif

		#if defined(TCCR4B) && defined(TCCR4A) && defined(WGM43) && defined(WGM42) && defined(WGM41) && defined(WGM40)
			TIMER_4BIT_WAVEFORM(4, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
		#endif
	}
#endif

	// init Timer5, presclae = 64, waveform = phase correct pwm
	{
		#if defined(TCCR5B) && defined(CS52) && defined(CS51) && defined(CS50)
			TIMER_CS(TCCR5B, 5, TIMER_WITH_EXT_CLK_CS_64)
		#endif

		#if defined(TCCR5B) && defined(TCCR5A) && defined(WGM53) && defined(WGM52) && defined(WGM51) && defined(WGM50)
			TIMER_4BIT_WAVEFORM(5, TIMER_4BIT_WAVEFROM_PCPWM_8BIT);
		#endif
	}

#if defined(ADCSRA)
	// set a2d prescale factor to 128
	// 16 MHz / 128 = 125 KHz, inside the desired 50-200 KHz range.
	// XXX: this will not work properly for other clock speeds, and
	// this code should use F_CPU to determine the prescale factor.
	sbi(ADCSRA, ADPS2);
	sbi(ADCSRA, ADPS1);
	sbi(ADCSRA, ADPS0);

	// enable a2d conversions
	sbi(ADCSRA, ADEN);
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
