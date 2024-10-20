#ifndef UART_COMM_H
#define UART_COMM_H

#include "stdint.h"

void UART_Init(void);
void Message_handler(uint8_t rx_buff[]);

#endif // UART_COMM_H
