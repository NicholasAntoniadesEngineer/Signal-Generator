/**
  ******************************************************************************
  * @file           : stm32_bsp.h
  * @brief          : Header for STM32 BSP functions
  * @details        : Provides function prototypes for HAL and BSP operations
  * @date           : 2021
  * @author         : Nicholas Antoniades
  ******************************************************************************
  */

#ifndef STM32_BSP_H
#define STM32_BSP_H

#include "gpio.h"
#include "tim.h"
#include "dac.h"
#include "usart.h"

void BSP_HAL_Init(void);
void BSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void BSP_TIM_Base_Start(TIM_HandleTypeDef* htim);
void BSP_TIM_Base_Stop(TIM_HandleTypeDef* htim);
void BSP_TIM_Base_Init(TIM_HandleTypeDef* htim);
void BSP_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment);
void BSP_UART_Receive(UART_HandleTypeDef* huart, uint8_t* pData, uint16_t Size, uint32_t Timeout);

#endif // STM32_BSP_H 