/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  * @details        : Defines the app_state structure and function prototypes.
  * @date           : 2020
  * @author         : Nicholas Antoniades
  ******************************************************************************
  */

#ifndef MAIN_H
#define MAIN_H

#include <string.h>
#include <stdlib.h>

#include "signal_generation.h"
#include "uart_message_handler.h"

#define RX_BUFF_SIZE 12
#define TX_BUFF_SIZE 12

extern uint8_t rx_buff[RX_BUFF_SIZE];
extern uint8_t tx_buff[TX_BUFF_SIZE];

typedef struct {
    UART_HandleTypeDef* huart;
    uint16_t rx_size;
    uint16_t tx_size;
    uint32_t uart_timeout;
} uart_config_t;

struct app_config {
    signal_state_t signal;
    uart_config_t uart;
};

typedef struct {
    signal_state_t signal_state;
    uart_state_t uart_state;
} app_state_t;

#endif // MAIN_H 