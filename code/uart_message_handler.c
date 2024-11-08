/**
  ******************************************************************************
  * @file           : uart_comm.c
  * @brief          : UART communication functions
  * @author         : Nicholas Antoniades
  * @date           : 2021
  ******************************************************************************
  */

#include "uart_comm.h"
#include "stm32_bsp.h"
#include "gpio.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t* message_handler(uart_config* config) 
{
    if ((config->rx_buff[0] == '<') && (config->rx_buff[config->rx_size - 1] == '>')) 
    {
        BSP_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET); // Receive
        BSP_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET); // Busy
        BSP_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET); // Transmit

        return config->rx_buff;
    }
    return NULL;
}

