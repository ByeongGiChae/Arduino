#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__)
#define _MX_
#endif

#if defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__)
#define _MX8_
#endif

#if defined(__AVR_ATmega640__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__)
#define _MX0_
#endif

#if defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
#define _MX4_
#endif

#if defined(__AVR_ATtiny84__)
#define _TINY_
#endif

#if defined(_MX_)
	#define RAMSTART (0x60)
#elif defined(_MX8_) || defined(_MX4)
	#define RAMSTART (0x100)
#elif defined(_MX0_)
	#define RAMSTART (0x200)
#endif	

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega88__) 
	#define NRWWSTART (0xC00)
#elif defined(__AVR_ATmega16__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega164P__) 
	#define NRWWSTART (0x1C00)
#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32__) || defined(__AVR_ATMega324P__)
	#define NRWWSTART (0x3800)
#elif defined(__AVR_ATmega640__) || defined(__AVR_ATmega644P__)
	#define NRWWSTART (0x7000)
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284P__)
	#define NRWWSTART (0xF000)
#elif defined(__AVR_ATmega2561__) || defined(__AVR_ATmega2561__)
	#define NRWWSTART (0x1F00)
#endif	
	
#if defined (__AVR_ATmega644P__)
#define RAMSTART (0x100)
#define NRWWSTART (0xE000)
#elif defined(__AVR_ATtiny84__)
#define RAMSTART (0x100)
#define NRWWSTART (0x0000)
#endif
/* update NRWWSTART  and RAMSTART according to datasheet- weihong.guan */


#if defined(_MX_) || defined(_MX8_)

#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED         PINB5

/* Ports for soft UART */
#ifdef SOFT_UART
#define UART_PORT   PORTD
#define UART_PIN    PIND
#define UART_DDR    DDRD
#define UART_TX_BIT 1
#define UART_RX_BIT 0
#endif

#endif

#if defined(_MX_) 
  //Name conversion R.Wiersma
  #define UCSR0A	UCSRA
  #define UCSR0B	UCSRB
  #define UCSR0C	UCSRC
  #define U2X0		U2X
  #define RXEN0		RXEN
  #define TXEN0		TXEN
  #define UCSZ00	UCSZ0
  #define UCSZ01	UCSZ1
  #define UDR0 		UDR
  #define UBRR0L	UBRRL
  #define UDRE0 	UDRE
  #define RXC0		RXC
  #define FE0		FE
  #define TIFR1 	TIFR
  #define WDTCSR	WDTCR
#endif

#if defined(_MX_) && defined(WDTOE)
  #define WDCE 		WDTOE
#endif

/* Luminet support */
#if defined(_TINY_)

/* Red LED is connected to pin PA4 */ 
#define LED_DDR     DDRA
#define LED_PORT    PORTA
#define LED_PIN     PINA
#define LED         PINA4

/* Ports for soft UART - left port only for now. TX/RX on PA2/PA3 */
#ifdef SOFT_UART
#define UART_PORT   PORTA
#define UART_PIN    PINA
#define UART_DDR    DDRA
#define UART_TX_BIT 2
#define UART_RX_BIT 3
#endif

#endif

/* Sanguino support */
#if defined(_MX4_)
/* Onboard LED is connected to pin PB0 on Sanguino */ 
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED         PINB0

/* Ports for soft UART */
#ifdef SOFT_UART
#define UART_PORT   PORTD
#define UART_PIN    PIND
#define UART_DDR    DDRD
#define UART_TX_BIT 1
#define UART_RX_BIT 0
#endif
#endif

/* Mega support */
#if defined(_MX0_)
/* Onboard LED is connected to pin PB7 on Arduino Mega */ 
#define LED_DDR     DDRB
#define LED_PORT    PORTB
#define LED_PIN     PINB
#define LED         PINB7

/* Ports for soft UART */
#ifdef SOFT_UART
#define UART_PORT   PORTE
#define UART_PIN    PINE
#define UART_DDR    DDRE
#define UART_TX_BIT 1
#define UART_RX_BIT 0
#endif
#endif
