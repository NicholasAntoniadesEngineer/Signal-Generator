/**
  ******************************************************************************
  * @file           : stm32_bsp.h
  * @brief          : Combined Board Support Package header file
  * @details        : Encapsulates HAL and BSP functions for all prototypes
  * @date           : 2024
  * @author         : Nicholas Antoniades
  ******************************************************************************
  */

#ifndef STM32_BSP_H_
#define STM32_BSP_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

/* Configuration constants */
#define BSP_HMI_BUFFER_SIZE         12U
#define BSP_NUM_ADC_CHANNELS        3U
#define BSP_UART_TX_COMPLETE_FLAG   255U

/* BSP state type definition */
typedef struct {
    uint8_t hmiTxBuffer[BSP_HMI_BUFFER_SIZE];
    uint8_t hmiRxBuffer[BSP_HMI_BUFFER_SIZE];
    uint32_t adcValues[BSP_NUM_ADC_CHANNELS];
    float pressure;
    uint8_t toggleValue;
    uint8_t runFlag;
    float pressure_threshold;
} bsp_state_t;

/* Function Prototypes */

/* Initialization Functions */
void stm32_bsp_hal_init(void);
void stm32_bsp_init_state(bsp_state_t* state);

/* Peripheral Initialization Functions */
void stm32_bsp_adc_init(ADC_HandleTypeDef* hadc, uint32_t instance);
void stm32_bsp_i2c_init(I2C_HandleTypeDef* hi2c, uint32_t instance);
void stm32_bsp_uart_init(UART_HandleTypeDef* huart, uint32_t instance, uint32_t baudrate);
void stm32_bsp_dac_init(DAC_HandleTypeDef* hdac);
void stm32_bsp_tim_init(TIM_HandleTypeDef* htim, uint32_t instance);
void stm32_bsp_gpio_init(void);
void stm32_bsp_dma_init(void);

/* GPIO Functions */
void stm32_bsp_gpio_writepin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState stm32_bsp_gpio_readpin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* Timer Functions */
void stm32_bsp_tim_base_start(TIM_HandleTypeDef* htim);
void stm32_bsp_tim_base_stop(TIM_HandleTypeDef* htim);
void stm32_bsp_tim_base_init(TIM_HandleTypeDef* htim);
void stm32_bsp_tim_pwm_init(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t frequency);

/* UART Functions */
void stm32_bsp_uart_transmit(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);
void stm32_bsp_uart_receive(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);
void stm32_bsp_uart_transmit_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);
void stm32_bsp_uart_receive_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);

/* I2C Functions */
void stm32_bsp_i2c_read(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size);
void stm32_bsp_i2c_write(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size);

/* ADC Functions */
void stm32_bsp_adc_start(ADC_HandleTypeDef* hadc);
void stm32_bsp_adc_stop(ADC_HandleTypeDef* hadc);
void stm32_bsp_adc_start_dma(ADC_HandleTypeDef* hadc, uint32_t* buffer, uint32_t length);
void stm32_bsp_adc_stop_dma(ADC_HandleTypeDef* hadc);
uint32_t stm32_bsp_adc_readvalue(ADC_HandleTypeDef* hadc);

/* System Functions */
uint32_t stm32_bsp_gettick(void);
void stm32_bsp_delay(uint32_t delay);

#endif /* STM32_BSP_H_ */ 