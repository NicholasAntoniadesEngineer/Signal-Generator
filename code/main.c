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

static const struct app_config config = 
{
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
        .rx_size = RX_BUFF_SIZE,
        .tx_size = TX_BUFF_SIZE,
        .uart_timeout = 100
    }
};

static void init_config(app_state_t *state, const struct app_config *config)
{
    stm32_lib_sig_gen_init(&state->signal_1, &config->signal_1);
    stm32_lib_sig_gen_init(&state->signal_2, &config->signal_2);

    uart_message_handler_init(&state->uart_state, &config->uart);
}

static void main_loop(app_state_t *state)
{
    while (1)
    {
        stm32_bsp_uart_receive(state->uart_state.huart, state->uart_state.rx_buff, 
                        state->uart_state.rx_size);

        uint8_t* message = message_handler(&state->uart_state);

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