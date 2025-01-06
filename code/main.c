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

static app_state_t state;

static const struct app_config config = 
{
    .signal = {
        .freq_signal_1 = 1000,
        .freq_signal_2 = 1000,
        .channel_1_sine_scale = 0.68,
        .channel_2_sine_scale = 0.68,
        .sine_dc_offset = 300,
        .psc = 0,
        .period = 1,
        .res = 4096,
        .fclock = 90000000,
        .min_freq = 500,
        .max_freq = 20000,
        .min_amplitude = 1,
        .max_amplitude = 68,
        .Ns = 80,
        .tim2 = &htim2,
        .tim4 = &htim4,
        .dac = &hdac
    },
    
    .uart = {
        .huart = &huart1,
        .rx_size = RX_BUFF_SIZE,
        .tx_size = TX_BUFF_SIZE,
        .uart_timeout = 100
    }
};

static void init_config(app_state_t *state, const struct app_config *config)
{
    // Initialize signal state
    signal_generation_init(&state->signal_state, &config->signal);

    // Initialize UART state
    stm32_bsp_uart_init(&state->uart_state, &config->uart);
}

static void main_loop(app_state_t *state)
{
    while (1)
    {
        stm32_bsp_uart_receive(state->uart_state.huart, state->uart_state.rx_buff, 
                        state->uart_state.rx_size, state->uart_state.uart_timeout);

        uint8_t* message = message_handler(state);

        if (message != NULL) state_machine(message, &state->signal_state);
    }
}

int main(void)
{
    stm32_bsp_hal_init();

    init_config(&state, &config);

    main_loop(&state);
}