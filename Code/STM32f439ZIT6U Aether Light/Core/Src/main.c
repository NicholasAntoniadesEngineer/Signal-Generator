/* USER CODE BEGIN Header */
/*
------------------------------------------------------------------------------
   Project	: Ventilator control system
   File     : main.c
   Brief    : Source file for the main.c file. This software was developed
   	   	   	  on the NUCLEO F439ZI development board.
   Author 	: Nicholas Antoniades
------------------------------------------------------------------------------
*/
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// Variables to be initialized for UART: HMI
#define HMIBufferSize 12			  		 // UART buffer size
uint8_t HMI_tx_buff[HMIBufferSize] = {0};	 // UART Transmit buffer
uint8_t HMI_rx_buff[HMIBufferSize] = {0};	 // UART Receive buffer


// Variables to initialized for ADC
#define  numADCchannels 1 		       // Set this value to be the number of channels
uint32_t value_adc[numADCchannels];    // Buffer size, 32 bit for 12 bit ADC resolution
float pressure;						   // Variable to hold the ADC value for pressure

// Variables to be initialized for timer interrupts
uint8_t toggleValue;

// Variables required for functions
// Convert to 8-bit
uint8_t eightBitResult[4];

//serial comms
uint8_t buf[12];

// Control
uint8_t run = 0;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  while(run == 0){
	  __NOP();
  }


  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 13000;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_TIM_Base_Start_IT(&htim3);

  // Setting initial valve conditions
  HAL_GPIO_WritePin(GPIOB, Valve1_Pin, 0);
  HAL_GPIO_WritePin(GPIOB, Valve2_Pin, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  while (1)
  {

	  HAL_Delay(50);

	// Toggling DMA streams for messaging and reading ADC values
	if(toggleValue == 1){
		HAL_ADC_Stop_DMA(&hadc1); 		// Close ADC DMA stream
		UARTSendDMA(eightBitResult, pressure, HMI_tx_buff, HMI_rx_buff);
		toggleValue = 0;
	}else{
		HAL_UART_DMAStop(&huart1); // Close UART DMA stream
		HAL_ADC_Start_DMA(&hadc1, value_adc,numADCchannels);
		toggleValue = 1;
	}
  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){

	// Code to run inside of handler
	HAL_GPIO_TogglePin(GPIOB, Valve1_Pin);
	HAL_GPIO_TogglePin(GPIOB, Valve2_Pin);
	// Start interrupt again
	HAL_TIM_Base_Start_IT(&htim3);
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin){
	switch(GPIO_Pin){

		case ON_Pin:
			 run = 1;
		break;


		case OFF_Pin:

			__NOP();
		break;
	}
}

void UARTSendDMA(uint8_t eightBitResult[4], float pressure,	uint8_t HMI_tx_buff[HMIBufferSize], uint8_t HMI_rx_buff[HMIBufferSize]) {
	// Pressure value for comms
	CTBVTFEBV(eightBitResult, (uint16_t) pressure);
	HMI_tx_buff[0] = eightBitResult[0];
	HMI_tx_buff[1] = eightBitResult[1];
	HMI_tx_buff[11] = 255; // Good message check
	HAL_UART_Transmit(&huart1, HMI_tx_buff, HMIBufferSize, 0XFF);

	//HAL_UART_Transmit_DMA(&huart1, HMI_tx_buff, HMIBufferSize); // Send
	//HAL_UART_Receive_DMA(&huart1, HMI_rx_buff, HMIBufferSize); // Receive
}
void CTBVTFEBV(uint8_t *eightBitresult,uint32_t value){  //Convert a 32 bit value to 4 bytes
	uint8_t eightBitpart1 = 0;
	uint8_t eightBitpart2 = 0;
	uint8_t eightBitpart3 = 0;
	uint8_t eightBitpart4 = 0;
	int counter = 0;
    int digit;

	while (value > 0) {
	 digit = value % 10; // e.g. 13 % 10 = 3
	 switch(counter){
		 case 0:
			 eightBitpart1 = digit; // e.g. = 3
	 	 	 break;
		 case 1:
			 eightBitpart1 = eightBitpart1 + digit*10 ; // e.g. = 3 = 1*10
	 	 	 break;
		 case 2:
			 eightBitpart2 = digit;
	 	 	 break;
		 case 3:
			 eightBitpart2 = eightBitpart2 + digit*10;
	 	 	 break;
		 case 4:
			 eightBitpart3 = digit;
	 	 	 break;
		 case 5:
			 eightBitpart3 = eightBitpart3 + digit*10 ;
	 	 	 break;
		 case 6:
			 eightBitpart4 = digit;
	 	 	 break;
		 case 7:
			 eightBitpart4 = eightBitpart3 + digit*10;
	 	 	 break;
		default:
		 	 break;
	 }
	 counter ++;
	 value /= 10;
	}
	eightBitresult[0] = eightBitpart1;
	eightBitresult[1] = eightBitpart2;
	eightBitresult[2] = eightBitpart3;
	eightBitresult[3] = eightBitpart4;
}
uint8_t CheckSum(uint8_t CommandArray[]){
	uint8_t CheckSum = 0;

	for(int i = 0; i<8; i ++){
		CheckSum += CommandArray[i];
	}
	return CheckSum;
}
void HoneyWellPressure(uint8_t PressureBuffer[4], uint8_t StatusHoneywell,uint8_t PressureHoneywell, uint8_t TempHoneywell) {
	// Reading in from Honeywell pressure sensor

	// *** fix this ***//
	I2C_read(0x28 << 1, 0x28 << 1, PressureBuffer, PressureBuffer);		  // Read in status
	StatusHoneywell = (PressureBuffer[0] & 0xc0);
	PressureHoneywell = (((PressureBuffer[0] << 8) | PressureBuffer[1])); // Read in Pressure
	TempHoneywell = (((PressureBuffer[3] << 8) | PressureBuffer[4])); 	  // Read in Temperature
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	// Function that is called at the end of each ADC conversion.
	pressure = (((float)value_adc[0]*10)/(4096-2055)-10.0686)*10.1972;  // Pressure in kPa -> cm H2O
	//pressure = value_adc[0];
	HAL_ADC_Stop_DMA(&hadc1);
}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef *huart){
	//HAL_UART_Start_DMA(&huart1, HMI_rx_buff, HMIBufferSize);
	//HAL_UART_DMAStop(&huart1);

	if(huart == &huart1){
	  //HAL_GPIO_WritePin(UART_RSE_GPIO_Port,UART_RSE_Pin, 0); // Toggle RSE pin to send message over RS485
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//HAL_UART_Receive_DMA(&huart1, HMI_rx_buff, HMIBufferSize);

	// Messages recieved from TMCL
	if(huart == &huart1){
		//HAL_GPIO_WritePin(UART_RSE_GPIO_Port,UART_RSE_Pin, GPIO_PIN_SET); // Toggle RSE pin to send message over RS485
	}

    // Messages received from HMI
	if(huart == &huart1){
		   switch (HMI_rx_buff[0])
		   {
		       // Start ventilation
			   case 0:
					  break;

			   // Stop and reset
			   case 1:
					break;

			   //Update ventilation states
			   case 2:
					break;

			   // Calibrate system
			   case 3:
					break;

			   // Return current system state
			   case 4:
				   break;

			   default:	break;
		   }
	}

	HAL_UART_DMAStop(&huart1);	// Close DMA stream
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
