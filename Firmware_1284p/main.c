/*
* main.c
*
* Created: 2015/03/29
*  Author: Timothy Thompkins
*/

#define FOSC 20000000UL
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((( FOSC / 16) + ( USART_BAUDRATE / 2) ) / ( USART_BAUDRATE )) - 1)

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "serial_comms.h"

void pinConfig() {

	DDRB = 0b00001000;
	DDRC = 0b00000000;
	DDRD = 0b11100000;

}

int main(void)
{

		uint8_t data1 = 'm';
		uint8_t data2 = 'n';
		USART_Init_Interrupt(BAUD_PRESCALE);  // Initialise USART
		pinConfig(); //Initialize Pin Configuration

		while(1)
		{
				if (PINC & (1<<PC0)) {
					PORTB |= (1<<PB3);
					USART_SendByte(data1);
				}

				else {
					PORTB &= ~(1<<PB3);
				}


				if (PINC & (1<<PC7)) {
					PORTB |= (1<<PB3);
					USART_SendByte(data2);
				}
				else {
					PORTB &= ~(1<<PB3);
				}

		}
}
