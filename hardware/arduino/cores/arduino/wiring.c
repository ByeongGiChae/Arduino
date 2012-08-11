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

#define TIMER_16BIT_WAVEFROM_NORMAL			0
#define TIMER_16BIT_WAVEFROM_PCPWM_8BIT		1
#define TIMER_16BIT_WAVEFROM_PCPWM_9BIT		2
#define TIMER_16BIT_WAVEFROM_PCPWM_10BIT	3
#define TIMER_16BIT_WAVEFROM_CTC_OCR1A		4
#define TIMER_16BIT_WAVEFROM_FASTPWM_8BIT	5
#define TIMER_16BIT_WAVEFROM_FASTPWM_9BIT	6
#define TIMER_16BIT_WAVEFROM_FASTPWM_10BIT	7
#define TIMER_16BIT_WAVEFROM_PFCPWM_ICR1	8
#define TIMER_16BIT_WAVEFROM_PFCPWM_OCR1A	9
#define TIMER_16BIT_WAVEFROM_PCPWM_ICR1		10
#define TIMER_16BIT_WAVEFROM_PCPWM_OCR1A	11
#define TIMER_16BIT_WAVEFROM_CTC_ICR1		12
#define TIMER_16BIT_WAVEFROM_FASTPWM_ICR1	14
#define TIMER_16BIT_WAVEFROM_FASTPWM__OCR1A	15

#define TIMER_8BIT_WAVEFORM_NORMAL	0
#define TIMER_8BIT_WAVEFORM_PCPWM	1
#define TIMER_8BIT_WAVEFORM_CTC		2
#define TIMER_8BIT_WAVEFORM_FPWM	3


volatile unsigned long timer0_overflow_count = 0;

#define TIMER_CS(reg, n, clock_selection)	\
	dbi(reg, CS##n##2, bitRead(clock_selection, 2));	\
	dbi(reg, CS##n##1, bitRead(clock_selection, 1));	\
	dbi(reg, CS##n##0, bitRead(clock_selection, 0))		\

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

void init_timer0(uint8_t clock_selection)
{

#if defined(CS02) && defined(CS01) && defined(CS00)

	#if defined(TCCR0)
		TIMER_CS(TCCR0, 0, clock_selection);
	#elif defined(TCCR0B)
		TIMER_CS(TCCR0B, 0, clock_selection);
	#else
		#error Timer 0 reg not found
	#endif

#else
	#error Timer 0 prescale factor not set correctly
#endif

	// enable timer 0 overflow interrupt
#if defined(TIMSK) && defined(TOIE0)
	sbi(TIMSK, TOIE0);
#elif defined(TIMSK0) && defined(TOIE0)
	sbi(TIMSK0, TOIE0);
#else
#error	Timer 0 overflow interrupt not set correctly
#endif

	// on the ATmega168, timer 0 is also used for fast hardware pwm
	// (using phase-correct PWM would mean that timer 0 overflowed half as often
	// resulting in different millis() behavior on the ATmega8 and ATmega168)

#if defined(TCCR0) && defined(WGM01) && defined(WGM00)
	sbi(TCCR0, WGM01);
	sbi(TCCR0, WGM00);
#elif defined(TCCR0A) && defined(WGM01) && defined(WGM00)
	sbi(TCCR0A, WGM01);
	sbi(TCCR0A, WGM00);
#endif
}

void init_timer1(uint8_t clock_selection)
{
	// timers 1 and 2 are used for phase-correct hardware pwm
	// this is better for motors as it ensures an even waveform
	// note, however, that fast pwm mode can achieve a frequency of up
	// 8 MHz (with a 16 MHz clock) at 50% duty cycle

#if defined(TCCR1B) && defined(CS12) && defined(CS11) && defined(CS10)

	TIMER_CS(TCCR1B, 1, clock_selection);

#else
	#error Timer 1 prescale factor not set correctly
#endif

	// put timer 1 in 8-bit phase correct pwm mode
#if defined(TCCR1A) && defined(WGM10)
	sbi(TCCR1A, WGM10);
#endif
}

void init_timer2(uint8_t clock_selection)
{
	// set timer 2 prescale factor to clock_selection
	#if defined(CS22) && defined(CS21) && defined(CS20)

		#if defined(TCCR2)
			TIMER_CS(TCCR2, 2, clock_selection);
		#elif defined(TCCR2B)
			TIMER_CS(TCCR2B, 2, clock_selection);
		#else
			#error Timer 2 reg not found
		#endif

	#else
		#error Timer 2 prescale factor not set correctly
	#endif

		// put timer 1 in 8-bit phase correct pwm mode
	#if defined(TCCR2) && defined(WGM20)
		sbi(TCCR2, WGM20);
	#elif defined(TCCR2A) && defined(WGM20)
		sbi(TCCR2A, WGM20);
	#endif
}

void init()
{
	// this needs to be called before setup() or some functions won't
	// work there
	sei();

	init_timer0(TIMER_WITH_EXT_CLK_CS_64);
	init_timer1(TIMER_WITH_EXT_CLK_CS_64);
	init_timer2(TIMER_WITHOUT_EXT_CLK_CS_64);

#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
	TIMER_CS(TCCR3B, 3, TIMER_WITH_EXT_CLK_CS_64)
//	sbi(TCCR3A, WGM30);// put timer 3 in 8-bit phase correct pwm mode
#endif

#if defined(TCCR4A) && defined(TCCR4B) && defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
	sbi(TCCR4B, CS42); // set timer4 prescale factor to 64
	sbi(TCCR4B, CS41);
	sbi(TCCR4B, CS40);
	sbi(TCCR4D, WGM40);// put timer 4 in phase- and frequency-correct PWM mode
	sbi(TCCR4A, PWM4A);// enable PWM mode for comparator OCR4A
	sbi(TCCR4C, PWM4D);// enable PWM mode for comparator OCR4D
#else /* beginning of timer4 block for ATMEGA1280 and ATMEGA2560 */
#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
	TIMER_CS(TCCR4B, 4, TIMER_WITH_EXT_CLK_CS_64)
	sbi(TCCR4A, WGM40);// put timer 4 in 8-bit phase correct pwm mode
#endif
#endif /* end timer4 block for ATMEGA1280/2560 and similar */	

#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
	TIMER_CS(TCCR5B, 5, TIMER_WITH_EXT_CLK_CS_64)
	sbi(TCCR5A, WGM50);// put timer 5 in 8-bit phase correct pwm mode
#endif

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
