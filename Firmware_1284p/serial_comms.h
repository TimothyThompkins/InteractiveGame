/*
* serial_comms.h
*
* Created: 2015/03/29
*  Author: Timothy Thompkins
*/

#ifndef _serial_comms_h
#define _serial_comms_h

/* Function prototypes */
void USART_Init_Interrupt  (unsigned int);
void USART_Init	   				 (unsigned int);
void USART_SendByte     	 (unsigned int);

uint8_t *getSetup          (void);

#endif
