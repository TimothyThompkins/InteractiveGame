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

#include "serial.h"

/*
 Function Prototypes in Main:
*/
ISR(USART_RXC_vect);
void pinConfig(void);
int main(void);

/* Test Varialbes */
extern volatile uint8_t receivedByte;
uint8_t running_count = 0;
uint8_t tst_count = 0;
uint8_t tst_count_2 = 0;
uint8_t tst_value = 0;
uint8_t u8test = 0;

// Declare Buffers
u8buf_Rx RxBuffer;
u8buf_Tx TxBuffer;

// RX Complete interrupt service routine //USART_RX_vect on 328P
// ISR is triggered when RX is complete
ISR(USART0_RX_vect)
{
		uint8_t u8temp;
		u8temp = UDR0;

		RxBufferWrite(&RxBuffer, u8temp); // Writes data to buffer
}

void pinConfig(void) {
	DDRB = 0b00001000;
	DDRC = 0b00000000;
	DDRD = 0b11100000;
}

int main(void)
{

		// uint8_t test_comms = 'z';
		// uint8_t setupState = 1;
		// uint8_t *setupData; // Pointer to hold setup data

		USART_Init(BAUD_PRESCALE);  // Initialise USART
		pinConfig(); // Initialize Pin Configuration

		// Main loop
		while(1)
		{

			// while(setupState) // Wait in this while loop while waiting for user conig
			// {
			// 	setupData = getSetup();
			// 	if ((*setupData + 0) == 1) // 3rd Element of pointer is init flag. If true break while loop and start.
			// 	{
			// 		setupState = 0;
			// 	}
			// }

			// PORTB |= (1<<PB3);
			// _delay_ms(1000);
			// PORTB &= ~(1<<PB3);

			if (PINC & (1<<PC0)) {
				PORTB |= (1<<PB3);

				// u8temp = running_count == 0? 't' : 'm';
					if (tst_count < 1)
					{
						for(uint8_t i = RxBuffer.index; i > 0; i--)
						{
						u8test = RxBuffer.buffer[i-1];
						TxBufferWrite(&TxBuffer, u8test);
						tst_count++;
						}
					}
					running_count++;
			}
			else
			{
				PORTB &= ~(1<<PB3);
				tst_count = 0;
			}

			if (PINC & (1<<PC7))
			{
					if(tst_count_2 < 1){
					PORTB |= (1<<PB3);
					// uint8_t test_data = 8;
					// USART_Test_Transmit(tst_value);
					USART_Transmit(&TxBuffer);
					RxBufferReset(&RxBuffer);
					// USART_Transmit(&RxBuffer);
					tst_count_2++;
				}

			}
			else {
				PORTB &= ~(1<<PB3);
				tst_count_2 = 0;
			}

			// if (receivedByte == 'k') {
			// 	PORTB |= (1<<PB3);
			// }
			// else {
			// 	PORTB &= ~(1<<PB3);
			// }

		}
}
