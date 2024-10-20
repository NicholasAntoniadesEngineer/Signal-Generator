/* signal_generation.c */
#include "signal_generation.h"
#include "tim.h"
#include "math.h"
#include "dac.h"
#include "gpio.h"
#include "stdint.h"

double Channel_1_sine_scale = 0.68; // Sine scale values. Max value = sine_scaled*3.3. Will result in a deformed signal.
double Channel_2_sine_scale = 0.68; // Giving a max amplitude of 3.24V
int sine_dc_offset = 300; 		    // DC off set value (4096Bits/3300mV)*200mV = 248.24Bits. Check
#define PI 3.1415926			    // Definition of PI
int Freq_Signal_1 = 1000; 		    // Frequency of signal 1
int Freq_Signal_2 = 1000; 		    // Frequency of signal 2
int PSC;						    // Variable to hold the Pre-scaler value
int Period = 1;					    // Variable to hold the Period
uint32_t Fclock = 90000000;		    // APB1 Timer Clock Frequency
int Res = 4096;				        // DAC resolution.
#define Ns 80 			            // Number of samples, Adjusting Ns will affect the frequency of the output signal.
								    // NS 80 seems to work up to 25kHz

void SignalGen_Init(uint32_t* Channel_1_sine_val, uint32_t* Channel_2_sine_val) 
{
    set_clock_TIM2();
    set_clock_TIM4();
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start(&htim4);
    Get_channel_1_sine(Channel_1_sine_val);
    Get_channel_2_sine(Channel_1_sine_val);
    HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, Channel_1_sine_val, Ns, DAC_ALIGN_12B_R);
    HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_2, Channel_2_sine_val, Ns, DAC_ALIGN_12B_R);

    HAL_GPIO_WritePin(GPIOD, LED1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, LED2_Pin, GPIO_PIN_SET);
}

void Get_channel_1_sine(uint32_t* Channel_1_sine_val) 
{
    for (int i = 0; i < Ns; i++) 
    {
        Channel_1_sine_val[i] = ((sin(i * 2 * PI / Ns) + 1) * (Res / 2));
        Channel_1_sine_val[i] = sine_dc_offset + Channel_1_sine_scale * Channel_1_sine_val[i];
    }
}

void Get_channel_2_sine(uint32_t* Channel_2_sine_val) 
{
    for (int i = 0; i < Ns; i++) 
    {
        Channel_2_sine_val[i] = ((sin(i * 2 * PI / Ns) + 1) * (Res / 2));
        Channel_2_sine_val[i] = sine_dc_offset + Channel_2_sine_scale * Channel_2_sine_val[i];
    }
}

void set_clock_TIM2(void) 
{
    HAL_TIM_Base_Stop(&htim2);
    PSC = (Fclock / Ns) / (Freq_Signal_1 * (Period + 1)) - 1;
    htim2.Init.Period = Period;
    htim2.Init.Prescaler = PSC;
    HAL_TIM_Base_Init(&htim2);
    HAL_TIM_Base_Start(&htim2);
}

void set_clock_TIM4(void) 
{
    HAL_TIM_Base_Stop(&htim4);
    PSC = (Fclock / Ns) / (Freq_Signal_2 * (Period + 1)) - 1;
    htim4.Init.Period = Period;
    htim4.Init.Prescaler = PSC;
    HAL_TIM_Base_Init(&htim4);
    HAL_TIM_Base_Start(&htim4);
}
