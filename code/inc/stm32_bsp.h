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
void BSP_HAL_Init(void);
void BSP_Init_State(bsp_state_t* state);

/* GPIO Functions */
void BSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
GPIO_PinState BSP_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

/* Timer Functions */
void BSP_TIM_Base_Start(TIM_HandleTypeDef* htim);
void BSP_TIM_Base_Stop(TIM_HandleTypeDef* htim);
void BSP_TIM_Base_Init(TIM_HandleTypeDef* htim);
void BSP_TIM_PWM_Init(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t frequency);

/* UART Functions */
void BSP_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);
void BSP_UART_Receive(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);
void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);
void BSP_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size);

/* I2C Functions */
void BSP_I2C_Read(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size);
void BSP_I2C_Write(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size);

/* ADC Functions */
void BSP_ADC_Start(ADC_HandleTypeDef* hadc);
void BSP_ADC_Stop(ADC_HandleTypeDef* hadc);
void BSP_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* buffer, uint32_t length);
void BSP_ADC_Stop_DMA(ADC_HandleTypeDef* hadc);
uint32_t BSP_ADC_ReadValue(ADC_HandleTypeDef* hadc);

/* System Functions */
uint32_t BSP_GetTick(void);
void BSP_Delay(uint32_t delay);

/* Handle Declarations */
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DAC_HandleTypeDef hdac;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

#endif /* STM32_BSP_H_ */ 