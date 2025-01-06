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

void uart_message_handler_init(uart_state_t* uart_state, const uart_config_t* config) {
    // Initialize UART state
    uart_state->huart = config->huart;
    uart_state->rx_size = config->rx_size;
    uart_state->tx_size = config->tx_size;
    uart_state->uart_timeout = config->uart_timeout;
}

void uart_transmit_message(uart_state_t* uart_state, uint8_t* message, uint16_t size) {
    stm32_bsp_uart_transmit(uart_state->huart, message, size);
}

void uart_receive_message(uart_state_t* uart_state, uint8_t* buffer, uint16_t size) {
    stm32_bsp_uart_receive(uart_state->huart, buffer, size);
}

