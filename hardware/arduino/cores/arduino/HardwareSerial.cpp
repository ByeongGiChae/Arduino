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

#include "HardwareSerial.h"

#if (RAMEND < 1000)
#define SERIAL_BUFFER_SIZE 16
#else
#define SERIAL_BUFFER_SIZE 64
#endif

// Constructors ////////////////////////////////////////////////////////////////

HardwareSerial::HardwareSerial(volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
		volatile uint8_t *ucsra, volatile uint8_t *ucsrb, volatile uint8_t *udr,
		uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udrie, uint8_t u2x,
		uint8_t buff_size) :
		_ubrrh(ubrrh), _ubrrl(ubrrl), _ucsra(ucsra), _ucsrb(ucsrb), _udr(udr), _rxen(
				rxen), _txen(txen), _rxcie(rxcie), _udrie(udrie), _u2x(u2x), _buff_size(
				buff_size) {
	_tx_buff.index_write = 0;
	_tx_buff.index_read = 0;
	_tx_buff.buffer = (uint8_t *) malloc(sizeof(uint8_t) * _buff_size);

	_rx_buff.index_write = 0;
	_rx_buff.index_read = 0;
	_rx_buff.buffer = (uint8_t *) malloc(sizeof(uint8_t) * _buff_size);
}

HardwareSerial::~HardwareSerial() {
	free(_tx_buff.buffer);
	free(_rx_buff.buffer);
}

// Public Methods //////////////////////////////////////////////////////////////

void HardwareSerial::begin(unsigned long baudrate) {
	uint16_t baud_setting;
	sbi(*_ucsra, _u2x);

	baud_setting = (F_CPU / 4 / baudrate - 1) / 2;

	if (baud_setting > 4095) {
		cbi(*_ucsra, _u2x);
		baud_setting = (F_CPU / 8 / baudrate - 1) / 2;
	}

	// assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
	*_ubrrh = baud_setting >> 8;
	*_ubrrl = baud_setting;

	sbi(*_ucsrb, _rxen);
	sbi(*_ucsrb, _txen);
	sbi(*_ucsrb, _rxcie);
	cbi(*_ucsrb, _udrie);
}

void HardwareSerial::end() {
	// wait for transmission of outgoing data
	while (_tx_buff.index_write != _tx_buff.index_read)
		;

	cbi(*_ucsrb, _rxen);
	cbi(*_ucsrb, _txen);
	cbi(*_ucsrb, _rxcie);
	cbi(*_ucsrb, _udrie);
}

int HardwareSerial::available(void) {
	return (_buff_size + _rx_buff.index_write - _rx_buff.index_read)
			% _buff_size;
}

int HardwareSerial::peek(void) {
	return _rx_buff.index_write == _rx_buff.index_read ?
			-1 : _rx_buff.buffer[_rx_buff.index_read];
}

int HardwareSerial::read(void) {
	if (_rx_buff.index_write != _rx_buff.index_read) {
		uint8_t c = _rx_buff.buffer[_rx_buff.index_read];
		_rx_buff.index_read = (_rx_buff.index_read + 1) % _buff_size;
		return c;
	}
	return -1;
}

void HardwareSerial::flush() {
	while (_tx_buff.index_write != _tx_buff.index_read)
		;
}

size_t HardwareSerial::write(uint8_t c) {
	uint8_t i = (_tx_buff.index_write + 1) % _buff_size;

	while (i == _tx_buff.index_read)
		;

	_tx_buff.buffer[_tx_buff.index_write] = c;
	_tx_buff.index_write = i;

	sbi(*_ucsrb, _udrie);
	return 1;
}

void HardwareSerial::transmit() {
	if (_tx_buff.index_write == _tx_buff.index_read) {
		cbi(*_ucsrb, _udrie);
		// Buffer empty, so disable interrupts
	} else {
		// There is more data in the output buffer. Send the next byte
		uint8_t c = _tx_buff.buffer[_tx_buff.index_read];
		_tx_buff.index_read = (_tx_buff.index_read + 1) % _buff_size;

		*_udr = c;
	}
}

void HardwareSerial::receive() {
	uint8_t i = (_rx_buff.index_write + 1) % _buff_size;

	// if we should be storing the received character into the location
	// just before the tail (meaning that the head would advance to the
	// current location of the tail), we're about to overflow the buffer
	// and so we don't write the character or advance the head.
	if (i != _rx_buff.index_read) {
		_rx_buff.buffer[_rx_buff.index_write] = *_udr;
		_rx_buff.index_write = i;
	}
}

// Preinstantiate Objects //////////////////////////////////////////////////////
#if defined(UBRRH) || defined(UBRR0H)

#if defined(UBRRH)
HardwareSerial Serial(&UBRRH, &UBRRL, &UCSRA, &UCSRB, &UDR, RXEN, TXEN, RXCIE, UDRIE, U2X, SERIAL_BUFFER_SIZE);
#else
HardwareSerial Serial(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UDR0, RXEN0, TXEN0,
		RXCIE0, UDRIE0, U2X0, SERIAL_BUFFER_SIZE);
#endif

#if defined(USART_UDRE_vect)
ISR(USART_UDRE_vect)
#elif defined(USART0_UDRE_vect)
ISR(USART0_UDRE_vect)
#endif
{
	Serial.transmit();
}

#if defined(USART_RXC_vect)
ISR(USART_RXC_vect)
#elif defined(USART_RX_vect)
ISR(USART_RX_vect)
#elif defined(USART0_RX_vect)
ISR(USART0_RX_vect)
#endif
{
	Serial.receive();
}

void serialEvent() __attribute__((weak));
void serialEvent()
{
}
#define serialEvent_implemented
#endif

#if defined(UBRR1H)
HardwareSerial Serial1(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UDR1, RXEN1, TXEN1, RXCIE1, UDRIE1, U2X1, SERIAL_BUFFER_SIZE);

ISR(USART1_UDRE_vect)
{
	Serial1.transmit();
}

ISR(USART1_RX_vect)
{
	Serial1.receive();
}
void serialEvent1() __attribute__((weak));
void serialEvent1()
{}
#define serialEvent1_implemented
#endif

#if defined(UBRR2H)
HardwareSerial Serial2(&UBRR2H, &UBRR2L, &UCSR2A, &UCSR2B, &UDR2, RXEN2, TXEN2, RXCIE2, UDRIE2, U2X2, SERIAL_BUFFER_SIZE);

ISR(USART2_UDRE_vect)
{
	Serial2.transmit();
}

ISR(USART2_RX_vect)
{
	Serial2.receive();
}
void serialEvent2() __attribute__((weak));
void serialEvent2()
{}
#define serialEvent2_implemented
#endif

#if defined(UBRR3H)
HardwareSerial Serial3(&UBRR3H, &UBRR3L, &UCSR3A, &UCSR3B, &UDR3, RXEN3, TXEN3, RXCIE3, UDRIE3, U2X3, SERIAL_BUFFER_SIZE);

ISR(USART3_UDRE_vect)
{
	Serial3.transmit();
}

ISR(USART3_RX_vect)
{
	Serial3.receive();
}
void serialEvent3() __attribute__((weak));
void serialEvent3()
{}
#define serialEvent3_implemented
#endif

void serialEventRun(void) {
#ifdef serialEvent_implemented
	if (Serial.available())
	serialEvent();
#endif
#ifdef serialEvent1_implemented
	if (Serial1.available()) serialEvent1();
#endif
#ifdef serialEvent2_implemented
	if (Serial2.available()) serialEvent2();
#endif
#ifdef serialEvent3_implemented
	if (Serial3.available()) serialEvent3();
#endif
}

