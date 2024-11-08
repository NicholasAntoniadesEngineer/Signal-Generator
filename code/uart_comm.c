#include "uart_comm.h"
#include "gpio.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Setting up UART communications */
#define uartSize_rx 10
#define uartSize_tx 10

uint8_t rx_buff[uartSize_rx];
uint8_t tx_buff[uartSize_tx];

void UART_Init(void) 
{
    HAL_UART_Receive(&huart2, rx_buff, uartSize_rx, 100);
}

uint8_t* Message_handler(void) 
{
    // If statements to validate message integrity
    if ((rx_buff[0] == '<') && (rx_buff[9] == '>')) 
    {
        // Toggling transmission light
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET); // Receive
        HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET); // Busy
        HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET); // Transmit

        // Return the pointer to the buffer for the state machine to handle
        return rx_buff;
    }
    return NULL; // Return NULL if message is not valid
}

