/*
* serial_comms.c
*
* Created: 2015/03/29
*  Author: Timothy Thompkins
*/

#include <avr/interrupt.h>

#include "serial_comms.h"

volatile uint8_t receivedByte;

#define BUF_SIZE 20

//type definition of buffer structure

typedef struct
{
	//Array of chars
	uint8_t buffer[BUF_SIZE];
	//array element index
	uint8_t index;

}u8buf;

//RX Complete interrupt service routine //USART_RX_vect on 328P
ISR(USART0_RX_vect) {

		uint8_t u8temp;
		u8temp = UDR0;

		//Check if recieved period char or end of buffer
		if ((BufferWrite(&buf, u8temp)==1) || (u8temp=='.'))
		{
				UCSR0B &= ~((1<<RXEN0) | (1<<RXCIE0)); //Disable reception and RX Complete interrupt
				UCSR0B |= (1<<TXEN0) | (1<<UDRIE0); //Enable transmission and UDR0 empty interrupt
		}
}

//UDR0 Empty interrupt service routine
ISR(USART_UDRE_vect)
{
    //if index is not at start of buffer
		if (BufferRead(&buf, &UDR0)==1)
		{
		    //start over
		    //reset buffer
		    BufferInit(&buf);
		    //disable transmission and UDR0 empty interrupt
		    UCSR0B &= ~((1<<TXEN0) | (1<<UDRIE0));
		    //enable reception and RC complete interrupt
		    UCSR0B |= (1<<RXEN0) | (1<<RXCIE0);
		}
}

void USART0_Init(unsigned int ubps) {

		// Note: ubps = usart baud pre-scale
		UCSR0B = (1<<RXEN0) | (1<<TXEN0 | (1<<RXCIE0); // Enable receiver and transmitter circuitry
		UBRR0H = (unsigned char)(ubps>>8); // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
		UBRR0L = (unsigned char)ubps; // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register
		UCSR0C = (1<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00); // 1<<USBS0S = 2 stop bits. 1<<UCSZn0 = 8 bit data transmission (standard). Note UCSZ02 is 0 by default

		sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

void USART_SendByte(unsigned int data) {


		//TMT - Change data to *data so that we can recieve a pointer. for the length of the pointer, write out UDR0 until it's done transmiting data
		
		while((UCSR0A & (1<<UDRE0) ) == 0) {};	// Wait until last byte has been transmitted
		UDR0 = data; // Transmit data
}

// TMT Get data, save into an array. when array fills up, do something.
// uint8_t *getData(void){
//
// 	uint8_t local_pos_input = 1; // User input selection local vs. remote. Default True
// 	uint8_t load_old_game = 0; // User load old game. Default False
// 	uint8_t init_game = 0; // Holds boolean to init game sequence or not
//
// 	static int data[100]; // Must be static for main.c to reference this array
//
// 	for (uint8_t i = 0; i < 2; i++)
// 	{
//
// 		setup[i] = receivedByte;
//
// 	}
//
// 	return setup;
//
// }

// uint8_t *getSetup(void){
//
// 	uint8_t local_pos_input = 1; // User input selection local vs. remote. Default True
// 	uint8_t load_old_game = 0; // User load old game. Default False
// 	uint8_t init_game = 0; // Holds boolean to init game sequence or not
//
// 	static int setup[3]; // Must be static for main.c to reference this array
//
// 	for (uint8_t i = 0; i < 2; i++)
// 	{
// 		setup[i] = receivedByte;
// 	}
//
// 	return setup;
//
// }

/*
USART_ReceiveByte(){
	while ((UCSRA & (1 << RXC)) == 0);
	// Do nothing until data has been received and is ready to be read from UDR
	return(UDR); // return the byte
}
*/
