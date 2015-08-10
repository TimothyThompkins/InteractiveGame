/*
 * serial_comms.h
 *
 * Created: 2015/03/29
 *  Author: Timothy Thompkins
 */

#ifndef _serial_comms_h
#define _serial_comms_h

#include <avr/interrupt.h>

#define BUFF_SIZE 32 // 32 bit size buffer

// This is a data buffer that holds recieved data
typedef struct
{
        uint8_t buffer[BUFF_SIZE]; // Array of chars
        uint8_t index; // Array element index
}u8buf_Rx;

// This is a data buffer for data that is collected to be sent
typedef struct
{
        uint8_t buffer[BUFF_SIZE]; // Array of chars
        uint8_t index; // Array element index
}u8buf_Tx;

/* Function prototypes */
void USART_Init             (unsigned int ubps);
void USART_Transmit         (u8buf_Tx *buf);
void USART_Test_Transmit    (uint8_t u8data);

void RxBufferReset          (u8buf_Rx *buf);
void RxBufferWrite          (u8buf_Rx *buf, uint8_t u8data);
void TxBufferWrite          (u8buf_Tx *buf, uint8_t u8data);

uint8_t TxBufferRead        (u8buf_Tx *buf, volatile uint8_t *u8data);

#endif
