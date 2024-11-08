/**
  ******************************************************************************
  * @file           : state_machine.h
  * @brief          : Header for state machine functions
  * @details        : Provides function prototypes for state handling
  * @date           : 2020
  * @date           : Nicholas Antoniades
  ******************************************************************************
  */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <string.h>
#include <stdlib.h>

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
} signal_config;

void handle_state(uint8_t command, uint8_t rx_buff[], signal_config* config);

#endif // STATE_MACHINE_H 