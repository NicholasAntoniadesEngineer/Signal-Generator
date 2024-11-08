/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  * @author         : Nicholas Antoniades
  * @date           : 2021
  ******************************************************************************
  */

#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "signal_generation.h"
#include "uart_comm.h"
#include "state_machine.h"
#include "stm32_bsp.h"


/* Function prototypes */
static void init_config(app_state *state);
static void init_peripherals(void);
static void main_loop(app_state *state);


/**
  * @brief  Initialize system configuration and HAL library.
  * @param  state: Pointer to the app_state structure.
  * @retval None
  */
static void init_config(app_state *state)
{
    state->signal_config = (signal_config){
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
        .channel_2_sine_val = {0}    
    };

    state->uart_config = (uart_config){
        .huart = &huart1,  
        .rx_buff = rx_buff,
        .tx_buff = tx_buff,
        .rx_size = sizeof(rx_buff),
        .tx_size = sizeof(tx_buff)
    };
}


/**
  * @brief  Main loop to handle incoming messages and update signal configuration.
  * @param  state: Pointer to the app_state structure.
  * @retval None
  */
static void main_loop(app_state *state)
{
    while (1)
    {
        BSP_UART_Receive(&huart2, state->uart_config.rx_buff, state->uart_config.rx_size, 100);

        uint8_t* message = message_handler(&state->uart_config);

        if (message != NULL) {
            handle_state(message[2], message, &state->signal_config);
        }
    }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    app_state state;
    init_config(&state);

    init_peripherals();

    signal_generation_init(&state.signal_config);

    main_loop(&state);
}