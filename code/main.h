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

#ifndef MAIN_H_
#define MAIN_H_

#include "stm32_lib.h"
#include "stm32_bsp.h"

#define RX_BUFF_SIZE 256
#define TX_BUFF_SIZE 256

typedef struct {
    stm32_sig_gen_state_t signal_1;
    stm32_sig_gen_state_t signal_2;
    uart_state_t uart_state;
} app_state_t;

struct app_config {
    stm32_sig_gen_state_t signal_1;
    stm32_sig_gen_state_t signal_2;
    struct {
        UART_HandleTypeDef *huart;
        uint16_t rx_size;
        uint16_t tx_size;
        uint32_t uart_timeout;
    } uart;
};

#endif /* MAIN_H_ */ 