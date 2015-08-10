/*
* serial_comms.c
*
* Created: 2015/03/29
*  Author: Timothy Thompkins
*/

#include "serial_comms.h"

// #define BUFF_SIZE 32 // 32 bit size buffer
// volatile uint8_t receivedByte;
// #define BUFFER_SIZE 100			// size of Rx ring buffer.
//type definition of buffer structure


void USART_Init(unsigned int ubps)
{
		// Note: ubps = usart baud pre-scale
		// TMT REGISTER = 1<<SOMETHING SOMETHING refers to a location in the register of UCSR0B, and we're shifting 1 into that place. That is to say SOMETHING is actually a number
		UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); // Enable receiver and transmitter circuitry
		UBRR0H = (unsigned char)(ubps>>8); // Load upper 8- bits of the baud rate value into the high byte of the UBRR register
		UBRR0L = (unsigned char)ubps; // Load lower 8 - bits of the baud rate value into the low byte of the UBRR register
		UCSR0C = (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00); // 1<<USBS0S = 2 stop bits. 0 = 1 stop bit. 1<<UCSZn0 = 8 bit data transmission (standard). Note UCSZ02 is 0 by default

		sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed
}

void USART_Transmit(u8buf_Tx *buf)
{
	while(TxBufferRead(buf, &UDR0) == 0) {}; // Will run until TxBufferRead Returns 1.
}

// TMT This is just to test serial directly
void USART_Test_Transmit(uint8_t u8data)
{
	while((UCSR0A & (1<<UDRE0) ) == 0) {};	// Wait until last byte has been transmitted
	UDR0 = u8data; // Transmit data
}

// Initialize buffer. Resets index to 0
void RxBufferReset(u8buf_Rx *buf)
{
		buf->index = 0; // Array element index
}

// Initialize buffer. Resets index to 0
void TxBufferReset(u8buf_Tx *buf)
{
		buf->index = 0; // Array element index
}

// Checks if space is left. If so writes data to buffer. If not return error (1)
void RxBufferWrite(u8buf_Rx *buf, uint8_t u8data)
{
		if (buf->index < BUFF_SIZE)
		{
				buf->buffer[buf->index] = u8data; // Put data u8data into buffer at location index
				// TMT In english: buf->index gets the member called index from the struct that buf points to.
				// That would be the current value of buffer. Therefore buf->buffer[buf->index]
				// gets the member buffer[current index value] from the struct that buf points to.
				// Into that address it places whatever was passed by u8data.
				buf->index++; // Increment buffer index by 1
				// return 0;
		}
		//else return 1; TMT if you want to invoke an error, comment these in, change type to uint8_t
}

// Checks if space is left. If so writes data to buffer.
void TxBufferWrite(u8buf_Tx *buf, uint8_t u8data)
{
		if (buf->index <  BUFF_SIZE)
		{
				buf->buffer[buf->index] = u8data; // Put data u8data into buffer at location index
				// TMT In english: buf->index gets the member called index from the struct that buf points to.
				// That would be the current value of buffer. Therefore buf->buffer[buf->index]
				// gets the member buffer[current index value] from the struct that buf points to.
				// Into that address it places whatever was passed by u8data.
				buf->index++; // Increment buffer index by 1
				//return 0;
		}
		// else return 1;
}

// Checks if space is left. If so reads data from buffer. If not return error (1). u8data pointer must be volatile
// uint8_t RxBufferRead(u8buf_Rx *buf)
// {
// 		uint8_t u8data = 0;
//
// 		if(buf->index > 0)
// 		{
// 				buf->index--; // Decrement buffer index by 1
// 				u8data = buf->buffer[buf->index]; // Derefernce that which is located
// 				// TMT this is the same as above, but instead of setting the value of buffer[buf->current index]
// 				// change the memory at the address in u8data to be that value.
// 				return u8data; // TMT potential pitfall Maybe needs to be &
// 		}
// 		// RxBufferReset(&buf); // Reset the tx buffer when done
// 		else return 1;
// }

// Checks if space is left. If so reads data from buffer. If not return error (1). u8data pointer must be volatile
uint8_t TxBufferRead(u8buf_Tx *buf, volatile uint8_t *u8data)
{
		if(buf->index > 0)
		{
				buf->index--; // Decrement buffer index by 1
				while((UCSR0A & (1<<UDRE0) ) == 0) {};	// Wait until last byte has been transmitted
				*u8data = buf->buffer[buf->index];
				// USART_Test_Transmit(buf->buffer[buf->index]);
				// TMT this is the same as above, but instead of setting the value of buffer[buf->current index]
				// change the memory at the address in u8data to be that value.
				return 0;
		}
		else
		{
				//TxBufferReset(buf); // Reset the tx buffer when done
				return 1;
		}
}

// // RX Complete interrupt service routine //USART_RX_vect on 328P
// // ISR is triggered when RX is complete
// ISR(USART0_RX_vect) {
//
// 		uint8_t u8temp;
// 		u8temp = UDR0;
//
// 		RxBufferWrite(&buf, u8temp); // Writes data to buffer
//
// 		// //Check if recieved period char or end of buffer
// 		// if ((BufferWrite(&buf, u8temp) == 1) || (u8temp == '.'))
// 		// {
// 		// 		UCSR0B &= ~((1<<RXEN0) | (1<<RXCIE0)); // Disable reception and RX Complete interrupt
// 		// 		UCSR0B |= (1<<TXEN0) | (1<<UDRIE0); // Enable transmission and UDR0 empty interrupt
// 		// }
// }

// Returns data in read buffer
// uint8_t getRxBuffer(void)
// {
// 	While(RxBufferRead(&buf, &UDR0) == 0) {}; // Will run until RxBufferRead Returns 1. TMT Should write out all data until buffer is empty.
// 	if ((BufferWrite(&buf, u8temp) == 1) || (u8temp == '.'))
// }

//UDR0 Empty interrupt service routine  // USART0_UDRE_vect on Atmega328p
// ISR(USART0_UDRE_vect)
// {
//     // if index is not at start of buffer
// 		if (BufferRead(&buf, &UDR0) == 1)
// 		{
// 		    //start over
// 		    //reset buffer
// 		    BufferReset(&buf);
// 		    UCSR0B &= ~((1<<TXEN0) | (1<<UDRIE0)); // Disable transmission and UDR0 empty interrupt
// 		    UCSR0B |= (1<<RXEN0) | (1<<RXCIE0); // Enable reception and RC complete interrupt
// 		}
// }

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
