#include "uart_comm.h"
#include "gpio.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Setting up UART communications */
#define uartSize_rx 10
#define uartSize_tx 10

uint8_t rx_buff[uartSize_rx];
uint8_t tx_buff[uartSize_tx];

int temp_frequency;
int temp_sine_scale;
char temp_buff[uartSize_rx];

// ASCII interpretation
char amplitude_value[2];
char frquency_value[6];
int value_int;

// For debugging
int counter = 0;

void UART_Init(void) 
{
    HAL_UART_Receive(&huart2, rx_buff, uartSize_rx, 100);
}

void Message_handler(uint8_t rx_buff[], int *Freq_Signal_1, int *Freq_Signal_2, double *Channel_1_sine_scale, double *Channel_2_sine_scale) 
{
    // If statements to validate message integrity
    if ((rx_buff[0] == '<') && (rx_buff[9] == '>')) 
    {

        // Toggling transmission light
        HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET); // Receive
        HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET); // Busy
        HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, GPIO_PIN_RESET); // Transmit

        // Switch statements to respond accordingly
        switch (rx_buff[2]) {

            /* Change the frequency of DAC channel 1 */
            case '1': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                char frquency_value[6] = {rx_buff[3], rx_buff[4], rx_buff[5], rx_buff[6], rx_buff[7], rx_buff[8]};
                int value_int = atoi(frquency_value);
                if (value_int < 500) value_int = 500;
                if (value_int > 20000) value_int = 20000;
                *Freq_Signal_1 = value_int; // Use pointer to update frequency
                set_clock_TIM2(); // Update frequency

                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            /* Change the frequency of DAC channel 2 */
            case '2': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                char frquency_value[6] = {rx_buff[3], rx_buff[4], rx_buff[5], rx_buff[6], rx_buff[7], rx_buff[8]};
                int value_int = atoi(frquency_value);
                if (value_int < 500) value_int = 500;
                if (value_int > 20000) value_int = 20000;
                *Freq_Signal_2 = value_int; // Use pointer to update frequency
                set_clock_TIM4(); // Update frequency

                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            /* Change the amplitude of DAC channel 1 */
            case '3': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                char amplitude_value[2] = {rx_buff[7], rx_buff[8]};
                int value_int = atoi(amplitude_value);
                if (value_int < 1) value_int = 1;
                if (value_int > 68) value_int = 68;
                *Channel_1_sine_scale = (double)value_int / 100; // Use pointer to update amplitude
                Get_channel_1_sine(); // Update sine wave

                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            /* Change the amplitude of DAC channel 2 */
            case '4': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                char amplitude_value[2] = {rx_buff[7], rx_buff[8]};
                int value_int = atoi(amplitude_value);
                if (value_int < 1) value_int = 1;
                if (value_int > 68) value_int = 68;
                *Channel_2_sine_scale = (double)value_int / 100; // Use pointer to update amplitude
                Get_channel_2_sine(); // Update sine wave

                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            /* Request Voltage and Current measurement of channel 1 output */
            case '5': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                // Implement logic to read voltage and current for channel 1
                Build_Response(rx_buff, tx_buff); // Adjust response data
                Transmit_Response();
                break;
            }

            /* Request Voltage and Current measurement of channel 2 output */
            case '6': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                // Implement logic to read voltage and current for channel 2
                Build_Response(rx_buff, tx_buff); // Adjust response data
                Transmit_Response();
                break;
            }

            /* Request Temperature sensor 1 and 2 output */
            case '7': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                // Implement logic to read temperature sensors 1 and 2
                Build_Response(rx_buff, tx_buff); // Adjust response data
                Transmit_Response();
                break;
            }

            /* Acknowledge message received */
            case '8': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                // Acknowledge message received
                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            /* Bad message received */
            case '9': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                // Handle bad message received
                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            /* Request current system state */
            case 'A': {
                HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET); // Busy
                // Implement logic to return current system state
                Build_Response(rx_buff, tx_buff);
                Transmit_Response();
                break;
            }

            default:
                // Handle unknown commands
                HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_RESET); // Indicate failure
                break;
        }
    }
}

