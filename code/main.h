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

uint8_t rx_buff[RX_BUFF_SIZE];
uint8_t tx_buff[TX_BUFF_SIZE];

typedef struct {
    signal_config signal_config;
    uart_state uart_state;
} app_state;

void init_config(signal_config *signal_config_ptr, uart_state *uart_state);
void main_loop(app_state *state);

#endif // MAIN_H 