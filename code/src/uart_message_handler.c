/**
  ******************************************************************************
  * @file           : uart_comm.c
  * @brief          : UART communication functions
  * @author         : Nicholas Antoniades
  * @date           : 2020
  ******************************************************************************
  */
#include <string.h>
#include <stdlib.h>

#include "uart_message_handler.h"
#include "stm32_bsp.h"

uint8_t* message_handler(uart_state* state) 
{
    if ((state->rx_buff[0] == '<') && (state->rx_buff[state->rx_size - 1] == '>')) 
    {
        BSP_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET); // Receive
        BSP_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET); // Busy
        BSP_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET); // Transmit

        return state->rx_buff;
    }
    return NULL;
}

