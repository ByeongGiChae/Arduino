/*
 HardwareSerial.h - Hardware serial library for Wiring
 Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

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

 Modified 28 September 2010 by Mark Sproul

 Modified August 28 2012 by Weihong Guan (aGuegu) to convert to a pure c++ class
 */

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <inttypes.h>
#include <avr/interrupt.h>
#include "Stream.h"

struct ring_buff
{
	uint8_t *buffer;
	volatile uint8_t index_write;	// to receive the incoming byte
	volatile uint8_t index_read;	// to send byte
};

class HardwareSerial: public Stream
{
public:
	HardwareSerial(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
			volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
			volatile uint8_t *udr, uint8_t rxen, uint8_t txen, uint8_t rxcie,
			uint8_t udrie, uint8_t u2x, uint8_t buff_size);
	virtual ~HardwareSerial()
	{
	}
	;
	void begin(unsigned long baudrate);
	void end();
	virtual int available(void);
	virtual int peek(void);
	virtual int read(void);
	virtual void flush(void);
	virtual size_t write(uint8_t);
	//using Print::write; // pull in write(str) and write(buf, size) from Print
	void transmit();
	void receive();

	operator bool();

private:
	volatile uint8_t * const _ubrrh;
	volatile uint8_t * const _ubrrl;
	volatile uint8_t * const _ucsra;
	volatile uint8_t * const _ucsrb;
	volatile uint8_t * const _udr;
	const uint8_t _rxen;
	const uint8_t _txen;
	const uint8_t _rxcie;
	const uint8_t _udrie;
	const uint8_t _u2x;
	const uint8_t _buff_size;

	ring_buff _tx_buff;
	ring_buff _rx_buff;

};

#if defined(UBRRH) || defined(UBRR0H)
extern HardwareSerial Serial;
#elif defined(USBCON)
#include "USBAPI.h"
//  extern HardwareSerial Serial_;  
#endif
#if defined(UBRR1H)
extern HardwareSerial Serial1;
#endif
#if defined(UBRR2H)
extern HardwareSerial Serial2;
#endif
#if defined(UBRR3H)
extern HardwareSerial Serial3;
#endif

extern void serialEventRun(void) __attribute__((weak));

#endif
