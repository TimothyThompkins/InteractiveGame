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

//extern volatile uint8_t receivedByte;

void pinConfig() {

	DDRB = 0b00001000;
	DDRC = 0b00000000;
	DDRD = 0b11100000;

}

int main(void)
{

		uint8_t test_comms = 'z';
		uint8_t setupState = 1;
		uint8_t *setupData; // Pointer to hold setup data

		USART_Init_Interrupt(BAUD_PRESCALE);  // Initialise USART
		pinConfig(); // Initialize Pin Configuration

		while(1)
		{


			while(setupState) // Wait in this while loop while waiting for user conig
			{
				setupData = getSetup();
				if ((*setupData + 0) == 1) // 3rd Element of pointer is init flag. If true break while loop and start.
				{
					setupState = 0;
				}
			}

			PORTB |= (1<<PB3);
			_delay_ms(1000);
			PORTB &= ~(1<<PB3);

			// if (PINC & (1<<PC0)) {
			// 	PORTB |= (1<<PB3);
			// 	USART_SendByte(data1);
			// }
			//
			// else {
			// 	PORTB &= ~(1<<PB3);
			// }
			//
			// if (PINC & (1<<PC7)) {
			// 	PORTB |= (1<<PB3);
			// 	USART_SendByte(data2);
			// }
			// else {
			// 	PORTB &= ~(1<<PB3);
			// }
			//
			// if (receivedByte == 'k') {
			// 	PORTB |= (1<<PB3);
			// }
			// else {
			// 	PORTB &= ~(1<<PB3);
			// }

		}
}
