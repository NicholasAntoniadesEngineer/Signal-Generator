/**
  ******************************************************************************
  * @file           : signal_generation.h
  * @brief          : Header for signal generation functions
  * @details        : Provides function prototypes for signal generation
  * @date           : 2020
  * @date           : Nicholas Antoniades
  ******************************************************************************
  */

#ifndef SIGNAL_GEN_H
#define SIGNAL_GEN_H

#include <string.h>
#include <stdlib.h>
#include "stm32_bsp.h"

typedef struct {
    // Signal parameters
    double channel_1_sine_scale;
    double channel_2_sine_scale;
    int sine_dc_offset;
    int freq_signal_1;
    int freq_signal_2;
    int psc;
    int period;
    int res;
    uint32_t fclock;
    uint32_t channel_1_sine_val[80]; // Using fixed size for Ns
    uint32_t channel_2_sine_val[80];
    int min_freq;
    int max_freq;
    int min_amplitude;
    int max_amplitude;
    int Ns;

    // Hardware handles
    TIM_HandleTypeDef tim2;
    TIM_HandleTypeDef tim4;
    DAC_HandleTypeDef dac;
} signal_state_t;

#define PI 3.1415926

// Function prototypes
void signal_generation_init(signal_state_t* state);
void get_channel_1_sine(uint32_t* channel_1_sine_val, signal_state_t* state);
void get_channel_2_sine(uint32_t* channel_2_sine_val, signal_state_t* state);
void set_clock_tim2(signal_state_t* state);
void set_clock_tim4(signal_state_t* state);

#endif // SIGNAL_GEN_H
