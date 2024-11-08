#include "state_machine.h"
#include "gpio.h"
#include <stdlib.h>


void handle_state(uint8_t command, uint8_t rx_buff[], int *Freq_Signal_1, int *Freq_Signal_2, double *Channel_1_sine_scale, double *Channel_2_sine_scale)
{
    switch (command) {
        case '1': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            char frquency_value[6] = {rx_buff[3], rx_buff[4], rx_buff[5], rx_buff[6], rx_buff[7], rx_buff[8]};
            int value_int = atoi(frquency_value);
            if (value_int < 500) value_int = 500;
            if (value_int > 20000) value_int = 20000;
            *Freq_Signal_1 = value_int; // Use pointer to update frequency
            set_clock_TIM2(); // Update frequency
            break;
        }
        case '2': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            char frquency_value[6] = {rx_buff[3], rx_buff[4], rx_buff[5], rx_buff[6], rx_buff[7], rx_buff[8]};
            int value_int = atoi(frquency_value);
            if (value_int < 500) value_int = 500;
            if (value_int > 20000) value_int = 20000;
            *Freq_Signal_2 = value_int; // Use pointer to update frequency
            set_clock_TIM4(); // Update frequency
            break;
        }
        case '3': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            char amplitude_value[2] = {rx_buff[7], rx_buff[8]};
            int value_int = atoi(amplitude_value);
            if (value_int < 1) value_int = 1;
            if (value_int > 68) value_int = 68;
            *Channel_1_sine_scale = (double)value_int / 100; // Use pointer to update amplitude
            Get_channel_1_sine(); // Update sine wave
            break;
        }
        case '4': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            char amplitude_value[2] = {rx_buff[7], rx_buff[8]};
            int value_int = atoi(amplitude_value);
            if (value_int < 1) value_int = 1;
            if (value_int > 68) value_int = 68;
            *Channel_2_sine_scale = (double)value_int / 100; // Use pointer to update amplitude
            Get_channel_2_sine(); // Update sine wave
            break;
        }
        case '5': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            // Implement logic to read voltage and current for channel 1
            break;
        }
        case '6': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            // Implement logic to read voltage and current for channel 2
            break;
        }
        case '7': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            // Implement logic to read temperature sensors 1 and 2
            break;
        }
        case '8': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            // Acknowledge message received
            break;
        }
        case '9': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            // Handle bad message received
            break;
        }
        case 'A': {
            HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
            // Implement logic to return current system state
            break;
        }
        default:
            // Handle unknown commands
            HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET); // Indicate failure
            break;
    }
} 