/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include <string.h>
//#include "usbd_cdc_if.h" // this is needed to transmit a string of characters
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

/* Setting up signal generation */
int Res = 4096;				// DAC resolution.
#define Ns 200  			// Number of samples, Adjusting Ns will affect the frequency of the output signal.
uint32_t sine_val[Ns];  	// Buffer for all the sine bits.
double sine_scaled = 0.5; 	// Scale value. Max value = sine_scaled*3.3. Will result in a deformed signal. Giving a max amplitude of 3.24V
int sine_dc_offset = 600; 	// DC off set value (4096Bits/3300mV)*200mV = 248.24Bits. Chec
#define PI 3.1415926		// Definition of PI
int Freq_Signal_1 = 1000; 	// Frequency of signal 1
int Freq_Signal_2 = 2000; 	// Frequency of signal 2
int PSC;					// Tim2 Pre Scalar value
uint32_t Fclock = 90000000;	// APB1 Timer Clocks
int Period = 1;				// Tim2 Period


void get_sine_val(void){

	// Fsine = FtimerRTGO/Ns,   Fsine = F(timer trigger ouput)/(number of samples)
	// Vsine(x)=(sine(x*(2PI/ns)+1)*((0xFFF+1)/2), this is an adjusted formula to create a positive sine.

	for(int i=0;i<Ns;i++){
		sine_val[i] = ((sin(i*2*PI/Ns)+1)*((Res)/2)); // Sampling step = 2PI/ns
		sine_val[i] = sine_dc_offset + sine_scaled*sine_val[i];
	}
	sine_val[Ns] = 0;
}



void set_clock_TIM2(void){
	  // Fsine = FtimerRTGO/Ns,   Fsine = F(timer trigger ouput)/(number of samples)
	  // Adjust PSC and period in order to manipulate frequency.

	  PSC= (Fclock/Ns)/(Freq_Signal_1*(Period + 1) ) - 1;

	  htim2.Instance = TIM2;
	  htim2.Init.Period = Period; //+1
	  htim2.Init.Prescaler = PSC; //+1 // If this value is < 4 things start to behave funny.
	  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	  {
	    Error_Handler();
	  }
}

void set_clock_TIM4(void){
	  // Fsine = FtimerRTGO/Ns,   Fsine = F(timer trigger ouput)/(number of samples)
	  // Adjust PSC and period in order to manipulate frequency.

	  PSC= (Fclock/Ns)/(Freq_Signal_2*(Period + 1) ) - 1;

	  htim4.Instance = TIM4;
	  htim4.Init.Period = Period; //+1
	  htim4.Init.Prescaler = PSC; //+1 // If this value is < 4 things start to behave funny.
	  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	  {
	    Error_Handler();
	  }
}


/* Setting up UART communications*/
#define uartSize 9
uint8_t rx_buff[uartSize];
uint8_t tx_buff[uartSize];

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	// Do something

	HAL_UART_Receive_DMA(&huart1, rx_buff, uartSize); // Receive UART
}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef *huart){
//	int i = 100;
//	while(i>0){
//		i = i -1;
//	}

	HAL_UART_Receive_DMA(&huart1, rx_buff, uartSize); // Receive UART
}

/* Setting up USB communications*/
char txBuf[12];
uint8_t count = 1;
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
  MX_DAC_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */

  /* Sine function */
  set_clock_TIM2();						// Setting frequency of timer 2
  set_clock_TIM4();						// Setting frequency of timer 4
  HAL_TIM_Base_Start(&htim2);			// Start timer 2
  HAL_TIM_Base_Start(&htim4);			// Start timer 4
  get_sine_val();						// Call get sineval function
  HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, sine_val, Ns, DAC_ALIGN_12B_R); //Start DMA, passing list of sine values.
  HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_2, sine_val, Ns, DAC_ALIGN_12B_R); //Start DMA, passing list of sine values.



  /* Setting up USB communications*/
  //  char txBuf[8];
  //  uint8_t count = 1;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	//HAL_UART_Receive_DMA(&huart1, rx_buff, uartSize); //set correct UART handler
	HAL_Delay(300);
	strcpy((char*)tx_buff, "Hello!\r\n");
	HAL_UART_Transmit_DMA(&huart1, tx_buff, uartSize);


	/* LED TEST */
	//HAL_GPIO_TogglePin(GPIOD, LED1_Pin);
	//HAL_GPIO_TogglePin(GPIOD, LED2_Pin);
//	HAL_GPIO_TogglePin(GPIOD, LED3_Pin);
//	HAL_GPIO_TogglePin(GPIOD, LED4_Pin);
//	HAL_GPIO_TogglePin(GPIOD, LED5_Pin);


//	sprintf(txBuf, "%u\r\n", count);
//	count++;
//
//	if (count>100){
//		count = 1;
//	}
//
//	CDC_Transmit_FS((uint8_t *) txBuf, strlen(txBuf));


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 180;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
}

/* USER CODE BEGIN 4 */

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
