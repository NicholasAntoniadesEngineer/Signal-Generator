#ifndef SIGNAL_GEN_H
#define SIGNAL_GEN_H

#include "stdint.h"

void signal_generation_init(uint32_t* Channel_1_sine_val, uint32_t* Channel_2_sine_val);
void Get_channel_1_sine(void);
void Get_channel_2_sine(void);
void set_clock_TIM2(void);
void set_clock_TIM4(void);

#endif // SIGNAL_GEN_H
