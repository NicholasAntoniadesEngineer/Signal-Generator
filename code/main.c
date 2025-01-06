/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author         : Nicholas Antoniades
  * @date           : 2020
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>

#include "main.h"
#include "stm32_bsp.h"
#include "signal_generation.h"
#include "uart_message_handler.h"
#include "state_machine.h"

uint8_t rx_buff[RX_BUFF_SIZE];
uint8_t tx_buff[TX_BUFF_SIZE];

/**
  * @brief  Initialize system configuration and HAL library.
  * @param  state: Pointer to the app_state structure.
  * @retval None
  */
static void init_config(app_state_t *state)
{
    state->signal_state = (signal_state_t)
    {
        .freq_signal_1 = 1000,
        .freq_signal_2 = 1000,
        .channel_1_sine_scale = 0.68,
        .channel_2_sine_scale = 0.68,
        .sine_dc_offset = 300,
        .psc = 0,
        .period = 1,
        .res = 4096,
        .fclock = 90000000,
        .channel_1_sine_val = {0},   
        .channel_2_sine_val = {0},
        .min_freq = 500,
        .max_freq = 20000,
        .min_amplitude = 1,
        .max_amplitude = 68,
        .Ns = 80,
        .tim2 = htim2,
        .tim4 = htim4,
        .dac = hdac
    };

    state->uart_state = (uart_state_t)
    {
        .huart = &huart1,  
        .rx_buff = rx_buff,
        .tx_buff = tx_buff,
        .rx_size = RX_BUFF_SIZE,
        .tx_size = TX_BUFF_SIZE,
        .uart_timeout = 100
    };
}

/**
  * @brief  Main loop to handle incoming messages and update signal configuration.
  * @param  state: Pointer to the app_state structure.
  * @retval None
  */
static void main_loop(app_state_t *state)
{
    while (1)
    {
        BSP_UART_Receive(state->uart_state.huart, state->uart_state.rx_buff, state->uart_state.rx_size, state->uart_state.uart_timeout);

        uint8_t* message = message_handler(state);

        if (message != NULL) state_machine(message, &state->signal_state);
    }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  app_state_t state;

  init_config(&state);

  BSP_HAL_Init();

  init_peripherals();

  signal_generation_init(&state.signal_state);

  main_loop(&state);
}