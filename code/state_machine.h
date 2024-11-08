#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

void handle_state(uint8_t command, uint8_t rx_buff[], int *Freq_Signal_1, int *Freq_Signal_2, double *Channel_1_sine_scale, double *Channel_2_sine_scale);

#endif // STATE_MACHINE_H 