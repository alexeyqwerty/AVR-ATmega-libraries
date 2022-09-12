#ifndef UART_MC_H_
#define UART_MC_H_

#include <avr/io.h>

#if defined (__AVR_ATmega8__) || defined(__AVR_ATmega8A__)
	#define USART_RX_INTERRUPT USART_RXC_vect
	#define USART_TX_INTERRUPT USART_TXC_vect
	#define _UCSRC UCSRC
	#define _URSEL URSEL
	#define _UCSZ1 UCSZ1
	#define _UCSZ0 UCSZ0
	#define _UBRRH UBRRH
	#define _UBRRL UBRRL
	#define _UCSRB UCSRB
	#define _RXEN RXEN
	#define _TXEN TXEN
	#define _RXCIE RXCIE
	#define _TXCIE TXCIE
	#define _UDR UDR

#elif defined(__AVR_ATmega16__) || defined (__AVR_ATmega16A__)
	#define USART_RX_INTERRUPT USART_RXC_vect
	#define USART_TX_INTERRUPT USART_TXC_vect
	#define _UCSRC UCSRC
	#define _URSEL URSEL
	#define _UCSZ1 UCSZ1
	#define _UCSZ0 UCSZ0
	#define _UBRRH UBRRH
	#define _UBRRL UBRRL
	#define _UCSRB UCSRB
	#define _RXEN RXEN
	#define _TXEN TXEN
	#define _RXCIE RXCIE
	#define _TXCIE TXCIE
	#define _UDR UDR

#elif defined (__AVR_ATmega32__) || defined(__AVR_ATmega32A__)
	#define USART_RX_INTERRUPT USART_RXC_vect
	#define USART_TX_INTERRUPT USART_TXC_vect
	#define _UCSRC UCSRC
	#define _URSEL URSEL
	#define _UCSZ1 UCSZ1
	#define _UCSZ0 UCSZ0
	#define _UBRRH UBRRH
	#define _UBRRL UBRRL
	#define _UCSRB UCSRB
	#define _RXEN RXEN
	#define _TXEN TXEN
	#define _RXCIE RXCIE
	#define _TXCIE TXCIE
	#define _UDR UDR

#elif (defined __AVR_ATmega328__) || defined(__AVR_ATmega328P__)
	#define USART_RX_INTERRUPT USART_RX_vect
	#define USART_TX_INTERRUPT USART_TX_vect
	#define _UCSRC UCSR0C
	#define _UCSZ1 UCSZ01
	#define _UCSZ0 UCSZ00
	#define _UBRRH UBRR0H
	#define _UBRRL UBRR0L
	#define _UCSRB UCSR0B
	#define _RXEN RXEN0
	#define _TXEN TXEN0
	#define _RXCIE RXCIE0
	#define _TXCIE TXCIE0
	#define _UDR UDR0

#elif defined (__AVR_ATmega644__) || defined (__AVR_ATmega644A__) || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__)
	#define USART_RX_INTERRUPT USART0_RX_vect
	#define USART_TX_INTERRUPT USART0_TX_vect
	#define _UCSRC UCSR0C
	#define _UCSZ1 UCSZ01
	#define _UCSZ0 UCSZ00
	#define _UBRRH UBRR0H
	#define _UBRRL UBRR0L
	#define _UCSRB UCSR0B
	#define _RXEN RXEN0
	#define _TXEN TXEN0
	#define _RXCIE RXCIE0
	#define _TXCIE TXCIE0
	#define _UDR UDR0

#elif defined (__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__)
	#define USART_RX_INTERRUPT USART0_RX_vect
	#define USART_TX_INTERRUPT USART0_TX_vect
	#define _UCSRC UCSR0C
	#define _UCSZ1 UCSZ01
	#define _UCSZ0 UCSZ00
	#define _UBRRH UBRR0H
	#define _UBRRL UBRR0L
	#define _UCSRB UCSR0B
	#define _RXEN RXEN0
	#define _TXEN TXEN0
	#define _RXCIE RXCIE0
	#define _TXCIE TXCIE0
	#define _UDR UDR0

#endif

#endif /* UART_MC_H_ */