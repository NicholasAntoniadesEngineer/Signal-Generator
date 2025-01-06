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
#include "stm32_lib.h"
#include "uart_message_handler.h"
#include "state_machine.h"

static app_state_t state;

static const app_config_t config = {
    .signal_1 = {
        .timer = &htim2,
        .frequency = 1000,
        .amplitude = 50,
        .offset = 0,
        .channel = TIM_CHANNEL_3
    },
    .signal_2 = {
        .timer = &htim4,
        .frequency = 1000,
        .amplitude = 50,
        .offset = 0,
        .channel = TIM_CHANNEL_4
    },
    .uart = {
        .huart = &huart1,
        .rx_size = 256,
        .tx_size = 256,
        .uart_timeout = 100
    },
    .port_config = {
        .pins_a = (pin_config_t[]) {
            { .pin = 0, .mode = GPIO_MODE_OUTPUT_PP, .pull = GPIO_NOPULL },
            // Add more pin configurations as needed
        },
        .num_pins_a = 1, // Update this count as needed
        .pins_b = (pin_config_t[]) {
            { .pin = 1, .mode = GPIO_MODE_INPUT, .pull = GPIO_PULLUP },
        },
        .num_pins_b = 1 // Update this count as needed
    }
};

static void init_config(app_state_t *state, const app_config_t *config) {
    stm32_lib_sig_gen_init(&state->signal_1, &config->signal_1);
    stm32_lib_sig_gen_init(&state->signal_2, &config->signal_2);
    uart_message_handler_init(&state->uart_state, &config->uart);
    stm32_lib_init_ports(&config->port_config);
}

static void main_loop(app_state_t *state)
{
    while (1)
    {
        stm32_bsp_uart_receive(state->uart_state.huart, 
                               state->uart_state.rx_buff, 
                               state->uart_state.rx_size);

        uint8_t *message = message_handler(&state->uart_state);

        if (message != NULL) state_machine(message, state);
    }
}

int main(void)
{
    stm32_bsp_gpio_init();
    stm32_bsp_dma_init();

    init_config(&state, &config);

    main_loop(&state);
}