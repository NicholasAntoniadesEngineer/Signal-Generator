/*
 * stm32_lib.h
 *
 * Created on: 24 Apr 2020
 * Author: Nicholas Antoniades
 */

#ifndef STM32_LIB_H_
#define STM32_LIB_H_

#include <stdint.h>
#define STM32F051
#include "stm32f0xx.h"

// Pin masks
#define PA8_MASK  0x0100
#define PA9_MASK  0x0200
#define PA10_MASK 0x0400
#define PB12_MASK 0x1000
#define PB13_MASK 0x2000
#define PB14_MASK 0x4000
#define PB15_MASK 0x8000

// Signal generator state structure
typedef struct {
    TIM_HandleTypeDef *timer;
    uint32_t frequency;
    uint32_t amplitude;
    uint32_t offset;
    uint32_t channel;
} stm32_sig_gen_state_t;

// UART state structure
typedef struct {
    UART_HandleTypeDef *huart;
    uint16_t rx_size;
    uint16_t tx_size;
    uint32_t uart_timeout;
} stm32_uart_state_t;

// Port state structure
typedef struct {
    pin_config_t *pins_a;
    int num_pins_a;
    pin_config_t *pins_b;
    int num_pins_b;
} stm32_port_state_t;

// Function prototypes
void stm32_lib_init_ports(void);
void stm32_lib_init_pwm_tim(TIM_HandleTypeDef *htim, uint32_t channel, uint32_t frequency);
void stm32_lib_init_pwm(int frequency);
void stm32_lib_init_adc(void);
void stm32_lib_init_usart(UART_HandleTypeDef *huart);
void stm32_lib_init_usart1(void);
void stm32_lib_init_tim6(uint32_t arr, uint32_t psc);
int stm32_lib_adc_input(int pot, int resolution);
void stm32_lib_adc_awd_8bit(int ADC_channel, int ADC_Low_threshhold, int ADC_High_threshhold);
int stm32_lib_adc_awd_check(void);
int stm32_lib_adc_data(void);
void stm32_lib_delay(int time);
int stm32_lib_check_button_gpioa(int button);
int stm32_lib_check_button_gpiob(int button);
void stm32_lib_debounce(void);
void stm32_lib_tim6_set_psc(uint32_t psc);
void stm32_lib_tim6_set_arr(uint32_t arr);
void stm32_lib_ack_irq(void);
void stm32_lib_init_exti(void);
void stm32_lib_usart1_transmit(unsigned char DataToTx);
unsigned char stm32_lib_usart1_receive(void);
void stm32_lib_sig_gen_init(stm32_sig_gen_state_t *state);

#endif /* STM32_LIB_H_ */ 