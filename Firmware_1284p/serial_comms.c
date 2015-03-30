/*
* serial_comms.c
*
* Created: 2015/03/29
*  Author: Timothy Thompkins
*/

#include <avr/interrupt.h>

#include "serial_comms.h"

volatile uint8_t receivedByte;

ISR(USART0_RX_vect) {

	receivedByte = UDR0; // Fetch the received byte value into the variable "receivedByte"

}

void USART_Init_Interrupt(unsigned int ubps) {

	// ubps = usart baud pre-scale
	UCSR0B = (1<<RXEN0) | (1<<TXEN0); // Enable receiver and transmitter circuitry
	UBRR0H = (unsigned char)(ubps>>8); // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = (unsigned char)ubps; // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register
	UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00); // 1<<USBS0S = 2 stop bits. 1<<UCSZn0 = 8 bit data transmission (standard). Note UCSZ02 is 0 by default

	UCSR0B |= (1<<RXCIE0); // Enable the USART Recieve Complete interrupt ( USART_RXC )

	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

void USART_Init(unsigned int ubps) {

	// Note that ubps = usart baud pre-scale
	UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable receiver and transmitter circuitry
	UBRR0H = (unsigned char)(ubps>>8); // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
	UBRR0L = (unsigned char)ubps; // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register
	UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00); // 1<<USBS0S = 2 stop bits. 3<<UCSZ00 = 8 bit data transmission (standard)

}


void USART_SendByte(unsigned int data) {

	while((UCSR0A & (1<<UDRE0) ) == 0) {};	// Wait until last byte has been transmitted
	UDR0 = data; // Transmit data
}

/*
USART_ReceiveByte(){
	while ((UCSRA & (1 << RXC)) == 0);
	// Do nothing until data has been received and is ready to be read from UDR
	return(UDR); // return the byte
}
*/
