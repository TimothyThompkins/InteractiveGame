/*
 * serial_comms.c
 *
 * Created: 2015/03/29
 *  Author: Timothy Thompkins
 */

#include "serial.h"

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
								while((UCSR0A & (1<<UDRE0) ) == 0) {}; // Wait until last byte has been transmitted
								UDR0 = u8data; // Transmit data
}

// Initialize buffer. Resets index to 0
void RxBufferReset(u8buf_Rx *buf)
{
								buf->index = 0; // Array element index
}

// Checks if space is left. If so writes data to buffer. If not return error (1)
void RxBufferWrite(u8buf_Rx *buf, uint8_t u8data)
{
								if (buf->index < BUFF_SIZE)
								{
																buf->buffer[buf->index] = u8data; // Put data u8data into buffer at location index
																buf->index++; // Increment buffer index by 1
								}
}

// Checks if space is left. If so writes data to buffer.
void TxBufferWrite(u8buf_Tx *buf, uint8_t u8data)
{
								if (buf->index <  BUFF_SIZE)
								{
																buf->buffer[buf->index] = u8data; // Put data u8data into buffer at location index
																buf->index++; // Increment buffer index by 1
								}
}

// Checks if space is left. If so reads data from buffer. If not return error (1). u8data pointer must be volatile
uint8_t TxBufferRead(u8buf_Tx *buf, volatile uint8_t *u8data)
{
								if(buf->index > 0)
								{
																buf->index--; // Decrement buffer index by 1
																while((UCSR0A & (1<<UDRE0) ) == 0) {}; // Wait until last byte has been transmitted
																*u8data = buf->buffer[buf->index];
																// USART_Test_Transmit(buf->buffer[buf->index]);
																// TMT this is the same as above, but instead of setting the value of buffer[buf->current index]
																// change the memory at the address in u8data to be that value.
																return 0;
								}
								else
								{
																return 1;
								}
}
