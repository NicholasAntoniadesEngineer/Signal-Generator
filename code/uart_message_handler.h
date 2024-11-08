/**
  ******************************************************************************
  * @file           : uart_comm.h
  * @brief          : Header for UART communication functions
  * @details        : Provides function prototypes for UART operations
  * @date           : 2020
  * @date           : Nicholas Antoniades
  ******************************************************************************
  */

#ifndef UART_COMM_H
#define UART_COMM_H

#include <string.h>
#include <stdlib.h>

typedef struct {
    UART_HandleTypeDef* huart;
    uint8_t* rx_buff;
    uint8_t* tx_buff;
    uint16_t rx_size;
    uint16_t tx_size;
    uint16_t uart_timeout;
} uart_config;

uint8_t* message_handler(uart_config* config);

#endif // UART_COMM_H
