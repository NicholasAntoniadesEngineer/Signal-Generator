/**
  ******************************************************************************
  * @file           : stm32_bsp.c
  * @brief          : Board Support Package for STM32
  * @details        : Encapsulates HAL and BSP functions for easier management
  * @date           : 2023
  * @author         : Nicholas Antoniades
  ******************************************************************************
  */

#include "stm32_bsp.h"
#include "gpio.h"
#include "tim.h"
#include "dac.h"
#include "usart.h"

void BSP_HAL_Init(void) 
{
    HAL_Init();
    
    // Initialize peripherals
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
    MX_DAC_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();
    MX_USART2_UART_Init();
}

void BSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState) 
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

void BSP_TIM_Base_Start(TIM_HandleTypeDef* htim) 
{
    HAL_TIM_Base_Start(htim);
}

void BSP_TIM_Base_Stop(TIM_HandleTypeDef* htim) 
{
    HAL_TIM_Base_Stop(htim);
}

void BSP_TIM_Base_Init(TIM_HandleTypeDef* htim) 
{
    HAL_TIM_Base_Init(htim);
}

void BSP_DAC_Start_DMA(DAC_HandleTypeDef* hdac, uint32_t Channel, uint32_t* pData, uint32_t Length, uint32_t Alignment) 
{
    HAL_DAC_Start_DMA(hdac, Channel, pData, Length, Alignment);
}

void BSP_UART_Receive(UART_HandleTypeDef* huart, uint8_t* pData, uint16_t Size, uint32_t Timeout) 
{
    HAL_UART_Receive(huart, pData, Size, Timeout);
} 