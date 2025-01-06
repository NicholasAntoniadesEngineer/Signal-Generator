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

typedef struct {
    /* Signal generator states */
    stm32_sig_gen_state_t signal_1;
    stm32_sig_gen_state_t signal_2;
    uart_state_t uart_state;
} app_state_t;

struct app_config {
    stm32_sig_gen_state_t signal_1;
    stm32_sig_gen_state_t signal_2;
    stm32_uart_state_t uart;
};

#endif /* MAIN_H_ */ 