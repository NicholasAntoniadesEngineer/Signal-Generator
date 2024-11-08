/**
  ******************************************************************************
  * @file           : signal_generation.c
  * @brief          : Signal generation functioconfig->Ns for dual-channel arbitrary waveform generator
  * @details        : Initializes and generates sine wave signals with configurable frequency and amplitude
  *                  on two independent DAC channels. Supports frequencies up to 25kHz with 80 samples
  *                  per period. Uses DMA for efficient waveform output and timer-based timing control.
  * @date           : 2020
  * @date           : Nicholas Antoniades
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>

#include "signal_generation.h"
#include "stm32_bsp.h"

void signal_generation_init(signal_config* config) 
{
    BSP_TIM_Base_Start(&htim2);
    BSP_TIM_Base_Start(&htim4);
    get_channel_1_sine(config->channel_1_sine_val, config);
    get_channel_2_sine(config->channel_2_sine_val, config);
    BSP_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, config->channel_1_sine_val, config->Ns, DAC_ALIGN_12B_R);
    BSP_DAC_Start_DMA(&hdac, DAC1_CHANNEL_2, config->channel_2_sine_val, config->Ns, DAC_ALIGN_12B_R);

    BSP_GPIO_WritePin(GPIOD, LED1_Pin, GPIO_PIN_SET);
    BSP_GPIO_WritePin(GPIOD, LED2_Pin, GPIO_PIN_SET);
}

void get_channel_1_sine(uint32_t* channel_1_sine_val, signal_config* config) 
{
    for (int i = 0; i < config->Ns; i++) 
    {
        channel_1_sine_val[i] = ((sin(i * 2 * PI / config->Ns) + 1) * (config->res / 2));
        channel_1_sine_val[i] = config->sine_dc_offset + config->channel_1_sine_scale * channel_1_sine_val[i];
    }
}

void get_channel_2_sine(uint32_t* channel_2_sine_val, signal_config* config) 
{
    for (int i = 0; i < config->Ns; i++) 
    {
        channel_2_sine_val[i] = ((sin(i * 2 * PI / config->Ns) + 1) * (config->res / 2));
        channel_2_sine_val[i] = config->sine_dc_offset + config->channel_2_sine_scale * channel_2_sine_val[i];
    }
}

void set_clock_tim2(signal_config* config) 
{
    BSP_TIM_Base_Stop(&htim2);
    config->psc = (config->fclock / config->Ns) / (config->freq_signal_1 * (config->period + 1)) - 1;
    htim2.Init.Period = config->period;
    htim2.Init.Prescaler = config->psc;
    BSP_TIM_Base_Init(&htim2);
    BSP_TIM_Base_Start(&htim2);
}

void set_clock_tim4(signal_config* config) 
{
    BSP_TIM_Base_Stop(&htim4);
    config->psc = (config->fclock / config->Ns) / (config->freq_signal_2 * (config->period + 1)) - 1;
    htim4.Init.Period = config->period;
    htim4.Init.Prescaler = config->psc;
    BSP_TIM_Base_Init(&htim4);
    BSP_TIM_Base_Start(&htim4);
}
