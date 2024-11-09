/**
  ******************************************************************************
  * @file           : signal_generation.h
  * @brief          : Header for signal generation functions
  * @details        : Provides function prototypes for signal generation
  * @date           : 2020
  * @date           : Nicholas Antoniades
  ******************************************************************************
  */

#include <string.h>
#include <stdlib.h>

#ifndef SIGNAL_GEN_H
#define SIGNAL_GEN_H

typedef struct {
    double channel_1_sine_scale;
    double channel_2_sine_scale;
    int sine_dc_offset;
    int freq_signal_1;
    int freq_signal_2;
    int psc;
    int period;
    int res;
    uint32_t fclock;
    uint32_t channel_1_sine_val[Ns];
    uint32_t channel_2_sine_val[Ns];
    int min_freq;
    int max_freq;
    int min_amplitude;
    int max_amplitude;
    int Ns;
} signal_state;

#define PI 3.1415926	           

void signal_generation_init(signal_state* state);
void get_channel_1_sine(uint32_t* channel_1_sine_val, signal_state* state);
void get_channel_2_sine(uint32_t* channel_2_sine_val, signal_state* state);
void set_clock_tim2(signal_state* state);
void set_clock_tim4(signal_state* state);

#endif // SIGNAL_GEN_H
