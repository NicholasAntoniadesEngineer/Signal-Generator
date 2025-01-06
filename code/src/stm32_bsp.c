/**
  ******************************************************************************
  * @file           : stm32_bsp.c
  * @brief          : Combined Board Support Package implementation
  * @details        : Encapsulates HAL and BSP functions for all prototypes
  * @date           : 2024
  * @author         : Nicholas Antoniades
  ******************************************************************************
  */

#include "stm32_bsp.h"
#include "gpio.h"
#include "tim.h"
#include "dac.h"
#include "usart.h"
#include "adc.h"
#include "i2c.h"
#include "dma.h"

/* Initialization Functions */
void BSP_HAL_Init(void)
{
    HAL_Init();
    
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_I2C3_Init();
    MX_DAC_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_USART3_UART_Init();
}

void BSP_Init_State(bsp_state_t* state)
{
    /* Initialize state structure with default values */
    memset(state, 0, sizeof(bsp_state_t));
    state->pressure_threshold = 0.0f;
    state->runFlag = 0;
    state->toggleValue = 0;
}

/* GPIO Functions */
void BSP_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

GPIO_PinState BSP_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

/* Timer Functions */
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

void BSP_TIM_PWM_Init(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t frequency)
{
    /* Configure timer for PWM mode */
    TIM_OC_InitTypeDef sConfigOC = {0};
    
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = frequency / 2;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    
    HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, channel);
    HAL_TIM_PWM_Start(htim, channel);
}

/* UART Functions */
void BSP_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit(huart, data, size, HAL_MAX_DELAY);
}

void BSP_UART_Receive(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Receive(huart, data, size, HAL_MAX_DELAY);
}

void BSP_UART_Transmit_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit_DMA(huart, data, size);
}

void BSP_UART_Receive_DMA(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Receive_DMA(huart, data, size);
}

/* I2C Functions */
void BSP_I2C_Read(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size)
{
    HAL_I2C_Mem_Read(hi2c, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

void BSP_I2C_Write(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size)
{
    HAL_I2C_Mem_Write(hi2c, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

/* ADC Functions */
void BSP_ADC_Start(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start(hadc);
}

void BSP_ADC_Stop(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Stop(hadc);
}

void BSP_ADC_Start_DMA(ADC_HandleTypeDef* hadc, uint32_t* buffer, uint32_t length)
{
    HAL_ADC_Start_DMA(hadc, buffer, length);
}

void BSP_ADC_Stop_DMA(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Stop_DMA(hadc);
}

uint32_t BSP_ADC_ReadValue(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(hadc);
}

/* System Functions */
uint32_t BSP_GetTick(void)
{
    return HAL_GetTick();
}

void BSP_Delay(uint32_t delay)
{
    HAL_Delay(delay);
} 