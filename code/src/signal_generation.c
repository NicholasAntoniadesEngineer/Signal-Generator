/**
  ******************************************************************************
  * @file           : signal_generation.c
  * @brief          : Signal generation functions for dual-channel arbitrary waveform generator
  * @details        : Initializes and generates sine wave signals with configurable frequency and amplitude
  *                  on two independent DAC channels. Supports frequencies up to 25kHz with 80 samples
  *                  per period. Uses DMA for efficient waveform output and timer-based timing control.
  * @date           : 2020
  * @date           : Nicholas Antoniades
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "signal_generation.h"
#include "stm32_bsp.h"

void signal_generation_init(signal_state* state) 
{
    BSP_TIM_Base_Start(&state->tim2);
    BSP_TIM_Base_Start(&state->tim4);
    
    get_channel_1_sine(state->channel_1_sine_val, state);
    get_channel_2_sine(state->channel_2_sine_val, state);
    
    BSP_DAC_Start_DMA(&state->dac, DAC1_CHANNEL_1, state->channel_1_sine_val, state->Ns, DAC_ALIGN_12B_R);
    BSP_DAC_Start_DMA(&state->dac, DAC1_CHANNEL_2, state->channel_2_sine_val, state->Ns, DAC_ALIGN_12B_R);

    BSP_GPIO_WritePin(GPIOD, LED1_Pin, GPIO_PIN_SET);
    BSP_GPIO_WritePin(GPIOD, LED2_Pin, GPIO_PIN_SET);
}

void get_channel_1_sine(uint32_t* channel_1_sine_val, signal_state* state) 
{
    for (int i = 0; i < state->Ns; i++) 
    {
        channel_1_sine_val[i] = ((sin(i * 2 * PI / state->Ns) + 1) * (state->res / 2));
        channel_1_sine_val[i] = state->sine_dc_offset + state->channel_1_sine_scale * channel_1_sine_val[i];
    }
}

void get_channel_2_sine(uint32_t* channel_2_sine_val, signal_state* state) 
{
    for (int i = 0; i < state->Ns; i++) 
    {
        channel_2_sine_val[i] = ((sin(i * 2 * PI / state->Ns) + 1) * (state->res / 2));
        channel_2_sine_val[i] = state->sine_dc_offset + state->channel_2_sine_scale * channel_2_sine_val[i];
    }
}

void set_clock_tim2(signal_state* state) 
{
    BSP_TIM_Base_Stop(&state->tim2);
    
    // Calculate prescaler
    state->psc = (state->fclock / state->Ns) / (state->freq_signal_1 * (state->period + 1)) - 1;
    
    // Update timer parameters through BSP
    BSP_TIM_Set_Period(&state->tim2, state->period);
    BSP_TIM_Set_Prescaler(&state->tim2, state->psc);
    
    BSP_TIM_Base_Init(&state->tim2);
    BSP_TIM_Base_Start(&state->tim2);
}

void set_clock_tim4(signal_state* state) 
{
    BSP_TIM_Base_Stop(&state->tim4);
    
    // Calculate prescaler
    state->psc = (state->fclock / state->Ns) / (state->freq_signal_2 * (state->period + 1)) - 1;
    
    // Update timer parameters through BSP
    BSP_TIM_Set_Period(&state->tim4, state->period);
    BSP_TIM_Set_Prescaler(&state->tim4, state->psc);
    
    BSP_TIM_Base_Init(&state->tim4);
    BSP_TIM_Base_Start(&state->tim4);
}
