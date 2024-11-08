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

/* Define constants for signal frequencies and scales */
#define FREQ_SIGNAL_1 1000
#define FREQ_SIGNAL_2 1000
#define CHANNEL_1_SINE_SCALE 0.68
#define CHANNEL_2_SINE_SCALE 0.68

/* Configuration struct for signal parameters */
typedef struct {
    int freq_signal_1;
    int freq_signal_2;
    double channel_1_sine_scale;
    double channel_2_sine_scale;
} signal_config;

/* Function prototypes */
void initialize_system(signal_config *config);
void initialize_peripherals(void);
void main_loop(signal_config *config);


/**
  * @brief  Initialize system configuration and HAL library.
  * @param  config: Pointer to the signal_config structure.
  * @retval None
  */
void initialize_system(signal_config *config)
{
    /* Initialize signal configuration */
    config->freq_signal_1 = FREQ_SIGNAL_1;
    config->freq_signal_2 = FREQ_SIGNAL_2;
    config->channel_1_sine_scale = CHANNEL_1_SINE_SCALE;
    config->channel_2_sine_scale = CHANNEL_2_SINE_SCALE;

    /* Initialize the HAL Library */
    HAL_Init();
}

/**
  * @brief  Initialize all configured peripherals.
  * @retval None
  */
void initialize_peripherals(void)
{
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
    MX_DAC_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();
    MX_USART2_UART_Init();
    UART_Init();

}

/**
  * @brief  Main loop to handle incoming messages and update signal configuration.
  * @param  config: Pointer to the signal_config structure.
  * @retval None
  */
void main_loop(signal_config *config)
{
    while (1)
    {
        uint8_t* message = Message_handler();
        if (message != NULL) {
            handle_state(message[2], 
                         message, 
                         &config->freq_signal_1, 
                         &config->freq_signal_2, 
                         &config->channel_1_sine_scale, 
                         &config->channel_2_sine_scale);
        }
    }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    signal_config config;

    initialize_system(&config);

    initialize_peripherals();

    signal_generation_init(Channel_1_sine_val, Channel_2_sine_val);

    main_loop(&config);
}