/*
* Hello_World.c
*
* Created: 9/28/2014 1:23:27 PM
*  Author: TimothyThompkins
*/

#define F_CPU 20000000UL // 20MHz

#include <avr/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define 	UCSRA 	UCSR0A
#define 	UCSRB 	UCSR0B
#define 	UCSRC 	UCSR0C
#define 	UBRRH 	UBRR0H
#define 	UBRRL 	UBRR0L
#define 	UDRE 		UDRE0
#define 	UDR 		UDR0
#define 	RXC 		RXC0
#define 	TXEN		TXEN0
#define 	RXEN		RXEN0
#define 	RXCIE		RXCIE0

// Define baud rate
#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

// Define baud rate 9600 is a safe speed as it is fast enough for most things we will use this speed
#define USART_BAUDRATE 9600

// Define the prescaler to use. This is based on the clock speed the AVR is running
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void USART_Init(void){
	/*
	First set the BAUD rate, because this register is actually more than 8 bits
	we need to load the lower 8 bits and upper 8 bits independently from one another.
	We will load the lower 8 bits first and upper 8 bits after.
	Also the default frame format is 8 data bits, no parity and 1 stopbit (also known as 8n1))
	We will use this format so no change needed here
	*/
	UBRRL = BAUD_PRESCALE; // Load lower 8 bits
	UBRRH = (BAUD_PRESCALE >> 8); // Load upper 8 bits
	// Enable transmitter only
	// UCSRB = 1<<TXEN;
	UCSRB =(1<<RXEN0)|(1<<TXEN0);

}

void USART_SendByte(uint8_t data){

	// Wait until last byte has been transmitted
	while((UCSRA &(1<<UDRE)) == 0);

	// Transmit data
	UDR = data;
}

USART_ReceiveByte(){
	while ((UCSRA & (1 << RXC)) == 0);
	// Do nothing until data has been received and is ready to be read from UDR
	return(UDR); // return the byte
}

//volatile unsigned char value;
/* This variable is volatile so both main and RX interrupt can use it.
It could also be a uint8_t type */

/* Interrupt Service Routine for Receive Complete
NOTE: vector name changes with different AVRs see AVRStudio -
Help - AVR-Libc reference - Library Reference - <avr/interrupt.h>: Interrupts
for vector names other than USART_RXC_vect for ATmega32 */
/*
ISR(USART_RX_vect){

value = UDR;             //read UART register into value
PORTB = ~value;          // output inverted value on LEDs (0=on)
}

void USART_Init(void){
// Set baud rate
UBRRL = BAUD_PRESCALE;// Load lower 8-bits into the low byte of the UBRR register
UBRRH = (BAUD_PRESCALE >> 8);
Load upper 8-bits into the high byte of the UBRR register
Default frame format is 8 data bits, no parity, 1 stop bit
to change use UCSRC, see AVR datasheet

// Enable receiver and transmitter and receive complete interrupt
UCSRB = ((1<<TXEN)|(1<<RXEN) | (1<<RXCIE));
}


void USART_SendByte(uint8_t u8Data){

// Wait until last byte has been transmitted
while((UCSRA &(1<<UDRE)) == 0);

// Transmit data
UDR = u8Data;
}


// not being used but here for completeness
// Wait until a byte has been received and return received data
uint8_t USART_ReceiveByte(){
while((UCSRA &(1<<RXC)) == 0);
return UDR;
}
*/

int main(void)
{

	uint8_t data;
	USART_Init();  // Initialise USART
	//sei();         // enable all interrupts
	//value = 'A'; //0x41;

	//Pin I/O Definitions
	DDRB = 0b00000111;
	DDRC = 0b00000000;
	DDRD = 0b11100000;

	while(1)
	{
		//This is a hello world the LED's
		// for (uint8_t i = 5; i<=7; i++)
		// { //Start at 5 b/c the first LED is connected to PD5
		// PORTD ^= 1<<i;
		// _delay_ms(1000);//This is a 20ms delay
		// }
		//
		// for (uint8_t i = 0; i<=2; i++)
		// {
		// PORTB ^= 1<<i;
		// _delay_ms(1000);//This is a 20ms delay
		// }
		//
		// //USART_SendByte(value);  // send value
		// //data = USART_ReceiveByte();
		//
		// if (data == 'U')
		// {
		// 	//USART_SendByte('T');  // Send value
		// 	//_delay_ms(100);
		// }
		//
		// else
		// {
		// 	//USART_SendByte('N');  // Send value
		// 	//_delay_ms(100);
		// }

		if (PINC & (1<<PC2))
		{
			PORTD |= (1<<PD5);
		}
		else
		{
			PORTD &= ~(1<<PD5);
		}

		if (PINC & (1<<PC3))
		{
			PORTD |= (1<<PD6);
		}
		else
		{
			PORTD &= ~(1<<PD6);
		}
	}
}
