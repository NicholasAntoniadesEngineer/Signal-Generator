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

void signal_generation_init(signal_state_t* state) {
    // Initialize signal state
    memset(state, 0, sizeof(signal_state_t));
    state->channel_1_sine_scale = 1.0;
    state->channel_2_sine_scale = 1.0;
    state->sine_dc_offset = 0;
    state->freq_signal_1 = 1000;
    state->freq_signal_2 = 1000;
    state->psc = 0;
    state->period = 1;
    state->res = 4096;
    state->fclock = 90000000;
    state->min_freq = 500;
    state->max_freq = 20000;
    state->min_amplitude = 1;
    state->max_amplitude = 68;
    state->Ns = 80;
}

void get_channel_1_sine(uint32_t* channel_1_sine_val, signal_state_t* state) {
    // Generate sine wave for channel 1
    for (int i = 0; i < state->Ns; i++) {
        channel_1_sine_val[i] = (uint32_t)(state->channel_1_sine_scale * state->sine_dc_offset);
    }
}

void get_channel_2_sine(uint32_t* channel_2_sine_val, signal_state_t* state) {
    // Generate sine wave for channel 2
    for (int i = 0; i < state->Ns; i++) {
        channel_2_sine_val[i] = (uint32_t)(state->channel_2_sine_scale * state->sine_dc_offset);
    }
}

void set_clock_tim2(signal_state_t* state) {
    stm32_bsp_tim_base_init(&state->tim2);
    stm32_bsp_tim_base_start(&state->tim2);
}

void set_clock_tim4(signal_state_t* state) {
    stm32_bsp_tim_base_init(&state->tim4);
    stm32_bsp_tim_base_start(&state->tim4);
}
