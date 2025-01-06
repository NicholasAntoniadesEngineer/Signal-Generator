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
void stm32_bsp_hal_init(void)
{
    HAL_Init();
    
    stm32_bsp_gpio_init();
    stm32_bsp_dma_init();
}

void stm32_bsp_init_state(bsp_state_t* state)
{
    /* Initialize state structure with default values */
    memset(state, 0, sizeof(bsp_state_t));
    state->pressure_threshold = 0.0f;
    state->runFlag = 0;
    state->toggleValue = 0;
}

/* Peripheral Initialization Functions */
void stm32_bsp_adc_init(ADC_HandleTypeDef* hadc, uint32_t instance)
{
    hadc->Instance = (ADC_TypeDef*)instance;
    hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc->Init.Resolution = ADC_RESOLUTION_12B;
    hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc->Init.ScanConvMode = DISABLE;
    hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc->Init.ContinuousConvMode = DISABLE;
    hadc->Init.DMAContinuousRequests = DISABLE;
    hadc->Init.DiscontinuousConvMode = DISABLE;
    hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    
    HAL_ADC_Init(hadc);
}

void stm32_bsp_i2c_init(I2C_HandleTypeDef* hi2c, uint32_t instance)
{
    hi2c->Instance = (I2C_TypeDef*)instance;
    hi2c->Init.ClockSpeed = 100000;
    hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c->Init.OwnAddress1 = 0;
    hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c->Init.OwnAddress2 = 0;
    hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    
    HAL_I2C_Init(hi2c);
}

void stm32_bsp_uart_init(UART_HandleTypeDef* huart, uint32_t instance, uint32_t baudrate)
{
    huart->Instance = (USART_TypeDef*)instance;
    huart->Init.BaudRate = baudrate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;
    
    HAL_UART_Init(huart);
}

void stm32_bsp_dac_init(DAC_HandleTypeDef* hdac)
{
    hdac->Instance = DAC;
    
    HAL_DAC_Init(hdac);
}

void stm32_bsp_tim_init(TIM_HandleTypeDef* htim, uint32_t instance)
{
    htim->Instance = (TIM_TypeDef*)instance;
    htim->Init.Prescaler = 0;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = 0xFFFF;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    HAL_TIM_Base_Init(htim);
}

void stm32_bsp_gpio_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
}

void stm32_bsp_dma_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();
    
    HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
    HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
}

/* GPIO Functions */
void stm32_bsp_gpio_writepin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}

GPIO_PinState stm32_bsp_gpio_readpin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

/* Timer Functions */
void stm32_bsp_tim_base_start(TIM_HandleTypeDef* htim)
{
    HAL_TIM_Base_Start(htim);
}

void stm32_bsp_tim_base_stop(TIM_HandleTypeDef* htim)
{
    HAL_TIM_Base_Stop(htim);
}

void stm32_bsp_tim_base_init(TIM_HandleTypeDef* htim)
{
    HAL_TIM_Base_Init(htim);
}

void stm32_bsp_tim_pwm_init(TIM_HandleTypeDef* htim, uint32_t channel, uint32_t frequency)
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
void stm32_bsp_uart_transmit(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit(huart, data, size, HAL_MAX_DELAY);
}

void stm32_bsp_uart_receive(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Receive(huart, data, size, HAL_MAX_DELAY);
}

void stm32_bsp_uart_transmit_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Transmit_DMA(huart, data, size);
}

void stm32_bsp_uart_receive_dma(UART_HandleTypeDef *huart, uint8_t *data, uint16_t size)
{
    HAL_UART_Receive_DMA(huart, data, size);
}

/* I2C Functions */
void stm32_bsp_i2c_read(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size)
{
    HAL_I2C_Mem_Read(hi2c, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

void stm32_bsp_i2c_write(I2C_HandleTypeDef *hi2c, uint16_t dev_addr, uint16_t reg_addr, uint8_t *data, uint16_t size)
{
    HAL_I2C_Mem_Write(hi2c, dev_addr, reg_addr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

/* ADC Functions */
void stm32_bsp_adc_start(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Start(hadc);
}

void stm32_bsp_adc_stop(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Stop(hadc);
}

void stm32_bsp_adc_start_dma(ADC_HandleTypeDef* hadc, uint32_t* buffer, uint32_t length)
{
    HAL_ADC_Start_DMA(hadc, buffer, length);
}

void stm32_bsp_adc_stop_dma(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_Stop_DMA(hadc);
}

uint32_t stm32_bsp_adc_readvalue(ADC_HandleTypeDef* hadc)
{
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
    return HAL_ADC_GetValue(hadc);
}

/* System Functions */
uint32_t stm32_bsp_gettick(void)
{
    return HAL_GetTick();
}

void stm32_bsp_delay(uint32_t delay)
{
    HAL_Delay(delay);
} 