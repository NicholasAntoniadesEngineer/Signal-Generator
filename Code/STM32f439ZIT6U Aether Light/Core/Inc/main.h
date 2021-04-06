/* USER CODE BEGIN Header */
/*
------------------------------------------------------------------------------
   Project	: Ventilator control system
   File     : main.h
   Brief    : Header file for main.c
   Author 	: Nicholas Antoniades
------------------------------------------------------------------------------
*/
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void PWM_frequency_set(uint32_t Fpwm, uint32_t PSC);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define Valve1_Pin GPIO_PIN_13
#define Valve1_GPIO_Port GPIOB
#define Valve2_Pin GPIO_PIN_14
#define Valve2_GPIO_Port GPIOB
#define ON_Pin GPIO_PIN_4
#define ON_GPIO_Port GPIOB
#define ON_EXTI_IRQn EXTI4_IRQn
#define OFF_Pin GPIO_PIN_5
#define OFF_GPIO_Port GPIOB
#define OFF_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

void initializations(void);
void I2C_read(uint8_t Device_Addr, uint16_t Device_reg, uint8_t buff_tx[], uint8_t buff_rx[]);
void I2C_write(uint8_t Device_Addr, uint16_t Device_reg, uint8_t buff_tx[], uint8_t buff_rx[]);
//void CSBVTTEBV(uint8_t *result,uint16_t value);
void splitSixteenToEight(uint8_t *result, uint16_t Long);
float timeOverflow(void);
void directionToggle(void);
void directionToggleInverse(void);
uint8_t CheckSum(uint8_t CommandArray[]);
void CTBVTFEBV(uint8_t *eightBitresult,uint32_t value);
void TMCL_UART_Message(uint8_t moduleAddress,uint8_t commandNumber,uint8_t commandType,uint8_t motorNumber,uint32_t commandValue);
void HoneyWellPressure(uint8_t PressureBuffer[4], uint8_t StatusHoneywell,uint8_t PressureHoneywell, uint8_t TempHoneywell);
void SensirionFlow(uint8_t FlowBuffer[3], float FlowSensirionOld[20],float FlowSensirion);
void SensirionFlowSensor(uint8_t FlowBuffer[3], float FlowSensirion[20], float deltaFlow, float FlowSensirionTotal);
void UARTSendDMA(uint8_t eightBitResult[4], float pressure,	uint8_t HMI_tx_buff[], uint8_t HMI_rx_buff[]);
void IntegrationForVolume(float deltaFlow, float deltaTime,	float FlowSensirion[20], float TidalVolume[2]) ;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
