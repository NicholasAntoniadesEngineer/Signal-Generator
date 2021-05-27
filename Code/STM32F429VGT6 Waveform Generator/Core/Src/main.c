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
#include <stdio.h>
#include <stdlib.h>
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
int Res = 4096;				        // DAC resolution.
#define Ns 80 			            // Number of samples, Adjusting Ns will affect the frequency of the output signal.
								    // NS 80 seems to work up to 25kHz
uint32_t Channel_1_sine_val[Ns];    // Look up table for all the first sine wave.
uint32_t Channel_2_sine_val[Ns];    // Look up table for all the second sine wave.
double Channel_1_sine_scale = 0.65; // Sine scale values. Max value = sine_scaled*3.3. Will result in a deformed signal.
double Channel_2_sine_scale = 0.65; // Giving a max amplitude of 3.24V
int sine_dc_offset = 480; 		    // DC off set value (4096Bits/3300mV)*200mV = 248.24Bits. Check
#define PI 3.1415926			    // Definition of PI
int Freq_Signal_1 = 1000; 		    // Frequency of signal 1
int Freq_Signal_2 = 1000; 		    // Frequency of signal 2
int PSC;						    // Variable to hold the Pre-scaler value
int Period = 1;					    // Variable to hold the Period
uint32_t Fclock = 90000000;		    // APB1 Timer Clock Frequency


/* Setting up UART communications*/
#define uartSize_rx 10
#define uartSize_tx 10
uint8_t rx_buff[uartSize_rx];
uint8_t tx_buff[uartSize_tx];
int temp_frequency;
int temp_sine_scale;
char temp_buff[uartSize_rx];

// ASCII interpretation
char amplitude_value[2];
char frquency_value[6];
int value_int;

// for debugging
int counter = 0;

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  /* Perform initializations */
  Startup(Channel_1_sine_val, Channel_2_sine_val, tx_buff);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  /* Wait for instructions*/
      HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, 0); // Set MAX485 to listening
	  HAL_UART_Receive(&huart2, rx_buff, uartSize_rx, 100);
	  Message_handler(rx_buff);

	 // HAL_UART_Transmit(&huart2, rx_buff, strlen((char*)rx_buff), 100); // Send message in RS485

	  //HAL_UART_Receive(&huart2, rx_buff, uartSize_rx, HAL_MAX_DELAY);


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
void Startup(uint32_t Channel_1_sine_val[Ns], uint32_t Channel_2_sine_val[Ns],	uint8_t tx_buff[uartSize_tx]) {

	/* Creating the Sine waveform at the DACs outputs 1 and 2*/
	set_clock_TIM2(); // Setting frequency of timer 2
	set_clock_TIM4(); // Setting frequency of timer 4
	HAL_TIM_Base_Start(&htim2); // Start timer 2
	HAL_TIM_Base_Start(&htim4); // Start timer 4
	Get_channel_1_sine(); // Generate the sine wave look up table
	Get_channel_2_sine(); // Generate the  sine wave look up table
	HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_1, Channel_1_sine_val, Ns, DAC_ALIGN_12B_R); //Start the DAC DMA implementation for output 1.
	/* Any code between here will cause phase lag between the output signals. */
	HAL_DAC_Start_DMA(&hdac, DAC1_CHANNEL_2, Channel_2_sine_val, Ns, DAC_ALIGN_12B_R); //Start the DAC DMA implementation for output 2.

	/* Setting signal output indicators to on  */
	HAL_GPIO_WritePin(GPIOD, LED1_Pin, 1); // Turn LED1 on
	HAL_GPIO_WritePin(GPIOD, LED2_Pin, 1); // Turn LED2 on

	/* Setting up RS485 communications */
	HAL_GPIO_WritePin(MAX485_PWR_GPIO_Port, MAX485_PWR_Pin, 1); // Turn on the MAX485 chip

	/* Building start up message */
	tx_buff[0] = 0x3C; // |<|     : Start of message byte.
	tx_buff[1] = 36;   // |ADDR|  : Device Address byte.
	tx_buff[2] = 36;   // |CMD|   : Command byte.
	tx_buff[3] = 36;   // |DATA1| : Data byte 1.
	tx_buff[4] = 36;   // |DATA2| : Data byte 2.
	tx_buff[5] = 36;   // |DATA3| : Data byte 3
	tx_buff[6] = 36;   // |DATA4| : Data byte 4.
	tx_buff[7] = 36;   // |DATA5| : Data byte 5.
	tx_buff[8] = 36;   // |DATA6| : Data byte 6.
	tx_buff[9] = '>';  // |>|     : End of message byte.


	/* Send start up message */
	HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, 1); // Set MAX485 to transmitting
	HAL_UART_Transmit(&huart2, tx_buff, strlen((char*) tx_buff), HAL_MAX_DELAY); // Send message in RS485
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	//Message_handler(rx_buff);

}

void HAL_UART_TxCpltCallback (UART_HandleTypeDef *huart){
	// Clear rx buffer

}

void Message_handler(uint8_t rx_buff[]){

	// If statements to validate message integrity
	if((rx_buff[0] == '<') && (rx_buff[9] == '>')){

		// Toggling transmission light
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 1); // Receive
		HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0); // Busy
		HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0); // Transmit

		/*
		|<|	     : Start of message byte.
		|ADDR|() : Device Address byte.
		|CMD|()  : Command byte.
		|DATA1|  : Data byte 1.
		|DATA2|  : Data byte 2.
		|DATA3|  : Data byte 3.
		|DATA4|  : Data byte 4.
		|DATA5|  : Data byte 5.
		|DATA6|  : Data byte 6.
		|>|		 : End of message byte.

		e.g.
		e.g.
		e.g.
		e.g.
		e.g.
		e.g.
		 */

		// Switch statements to respond accordingly
		switch(rx_buff[2]){

			/* Change the frequency of DAC channel 1. */
			case '1':
				// Toggling transmission light
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1); // Busy

				// Building 4 bytes int a 6 digit value
				frquency_value[0] = rx_buff[3];
				frquency_value[1] = rx_buff[4];
				frquency_value[2] = rx_buff[5];
				frquency_value[3] = rx_buff[6];
				frquency_value[4] = rx_buff[7];
				frquency_value[5] = rx_buff[8];
				value_int = atoi(frquency_value);

				// Boundary checks
				if(value_int < 500){
					value_int = 500;
				}
				if(value_int > 20000){
					value_int = 20000;
				}
				// Updating channel 1 output frequency
				Freq_Signal_1 = value_int;
				set_clock_TIM2();	// Set the new sine frequency by updating the associate clock frequency


				// Calculating frequency from buffer value
				// temp_frequency  = ((Fclock/Ns)/((TIM2 ->PSC) - 1 )*(Period + 1));
				// itoa(temp_frequency, temp_buff,10);

				// Building response
				tx_buff[0] = '<'; 			// |<|     : Start of message byte.
				tx_buff[1] = rx_buff[1];	// |ADDR|  : Device Address byte.
				tx_buff[2] = rx_buff[2];	// |CMD|() : Command byte.
				tx_buff[3] = rx_buff[3];	// |DATA1| : Data byte 1.
				tx_buff[4] = rx_buff[4];	// |DATA2| : Data byte 2.
				tx_buff[5] = rx_buff[5];	// |DATA3| : Data byte 3
				tx_buff[6] = rx_buff[6];	// |DATA4| : Data byte 4.
				tx_buff[7] = rx_buff[7];	// |DATA1| : Data byte 5.
				tx_buff[8] = rx_buff[8];	// |DATA2| : Data byte 6.
				tx_buff[9] = '>'; 			// |>|     : End of message byte.

				// Toggling transmission light
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1); // Transmit

				// Sending response
			    HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, 1); 					// Set MAX485 to transmitting
				HAL_UART_Transmit(&huart2, tx_buff, strlen((char*)tx_buff), HAL_MAX_DELAY); // Send message in RS485
				counter++;
				break;

			/* Change the frequency of DAC channel 2. */
			case '2':
				// Toggling transmission light
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1); // Busy

				// Building 4 bytes int a 6 digi value
				frquency_value[0] = rx_buff[3];
				frquency_value[1] = rx_buff[4];
				frquency_value[2] = rx_buff[5];
				frquency_value[3] = rx_buff[6];
				frquency_value[4] = rx_buff[7];
				frquency_value[5] = rx_buff[8];
				value_int = atoi(frquency_value);

				// Boundary checks
				if(value_int < 500){
					value_int = 500;
				}
				if(value_int > 20000){
					value_int = 20000;
				}

				// Updating channel 1 output frequency
				Freq_Signal_2 = value_int;
				set_clock_TIM4();	// Set the new sine frequency by updating the associate clock frequency

				// Calculating frequency from buffer value
				// temp_frequency  = ((Fclock/Ns)/((TIM4 ->PSC) - 1 )*(Period + 1));
				// itoa(temp_frequency, temp_buff,10);

				// Building response
				tx_buff[0] = '<'; 			// |<|     : Start of message byte.
				tx_buff[1] = rx_buff[1];	// |ADDR|  : Device Address byte.
				tx_buff[2] = rx_buff[2];	// |CMD|() : Command byte.
				tx_buff[3] = rx_buff[3];	// |DATA1| : Data byte 1.
				tx_buff[4] = rx_buff[4];	// |DATA2| : Data byte 2.
				tx_buff[5] = rx_buff[5];	// |DATA3| : Data byte 3
				tx_buff[6] = rx_buff[6];	// |DATA4| : Data byte 4.
				tx_buff[7] = rx_buff[7];	// |DATA1| : Data byte 5.
				tx_buff[8] = rx_buff[8];	// |DATA2| : Data byte 6.
				tx_buff[9] = '>'; 			// |>|     : End of message byte.

				// Toggling transmission light
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1); // Transmit

				// Sending response
			    HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, 1); 					// Set MAX485 to transmitting
				HAL_UART_Transmit(&huart2, tx_buff, strlen((char*)tx_buff), HAL_MAX_DELAY); // Send message in RS485
				counter++;
				break;

			/* Change the amplitude of DAC channel 1 */
			case '3':
				// Toggling transmission light
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1); // Busy

				// Building 4 bytes int a 6 digit value
				amplitude_value[0] = rx_buff[7];
				amplitude_value[1] = rx_buff[8];
				value_int = atoi(amplitude_value);

				// Boundary checks
				if(value_int < 1){
					value_int = 1;
				}
				if(value_int > 68){
					value_int = 68;
				}


				// Updating channel 1 amplitude
				Channel_1_sine_scale = (double)value_int/100; // Dividing by 100 to create a fraction
				Get_channel_1_sine(); // Generate new table of sine values

				// Building response
				tx_buff[0] = '<'; 			// |<|     : Start of message byte.
				tx_buff[1] = rx_buff[1];	// |ADDR|  : Device Address byte.
				tx_buff[2] = rx_buff[2];	// |CMD|() : Command byte.
				tx_buff[3] = rx_buff[3];	// |DATA1| : Data byte 1.
				tx_buff[4] = rx_buff[4];	// |DATA2| : Data byte 2.
				tx_buff[5] = rx_buff[5];	// |DATA3| : Data byte 3
				tx_buff[6] = rx_buff[6];	// |DATA4| : Data byte 4.
				tx_buff[7] = rx_buff[7];	// |DATA1| : Data byte 5.
				tx_buff[8] = rx_buff[8];	// |DATA2| : Data byte 6.
				tx_buff[9] = '>'; 			// |>|     : End of message byte.

				// Toggling transmission light
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1); // Transmit

				// Sending response
			    HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, 1); 					// Set MAX485 to transmitting
				HAL_UART_Transmit(&huart2, tx_buff, strlen((char*)tx_buff), HAL_MAX_DELAY); // Send message in RS485

				counter++;
				break;


			/* Change the amplitude of DAC channel 2. */
			case '4':
				// Toggling transmission light
				HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1); // Busy

				// Building 4 bytes int a 6 digit value
				amplitude_value[0] = rx_buff[7];
				amplitude_value[1] = rx_buff[8];
				value_int = atoi(amplitude_value);

				// Boundary checks
				if(value_int < 1){
					value_int = 1;
				}
				if(value_int > 68){
					value_int = 68;
				}

				// Updating channel 2 amplitude
				Channel_2_sine_scale = (double)value_int/100; // Dividing by 100 to create a fraction
				Get_channel_2_sine();	// Generate new table of sine values


				// Building response
				tx_buff[0] = '<'; 			// |<|     : Start of message byte.
				tx_buff[1] = rx_buff[1];	// |ADDR|  : Device Address byte.
				tx_buff[2] = rx_buff[2];	// |CMD|() : Command byte.
				tx_buff[3] = rx_buff[3];	// |DATA1| : Data byte 1.
				tx_buff[4] = rx_buff[4];	// |DATA2| : Data byte 2.
				tx_buff[5] = rx_buff[5];	// |DATA3| : Data byte 3
				tx_buff[6] = rx_buff[6];	// |DATA4| : Data byte 4.
				tx_buff[7] = rx_buff[7];	// |DATA1| : Data byte 5.
				tx_buff[8] = rx_buff[8];	// |DATA2| : Data byte 6.
				tx_buff[9] = '>'; 			// |>|     : End of message byte.

				// Toggling transmission light
				HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 1); // Transmit

				// Sending response
			    HAL_GPIO_WritePin(Direction_GPIO_Port, Direction_Pin, 1); 					// Set MAX485 to transmitting
				HAL_UART_Transmit(&huart2, tx_buff, strlen((char*)tx_buff), HAL_MAX_DELAY); // Send message in RS485
				counter++;
				break;

			/* Request Voltage and Current measurement of channel 1 output. */
			/* Request Voltage and Current measurement of channel 2 output. */
			/* Request Temperature sensor 1 and 2 output. */
			/* Acknowledge message received. */
			/* Bad message received. */
			/* Request current system state. */

		}


	}

	// Clear rx buffer
	for(int i = 0; i<uartSize_rx; i++){
		rx_buff[i] = 0;
		temp_buff[i] = 0;
	}

	// Toggling transmission light
	HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, 0); // Receive
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 0); // Busy
	HAL_GPIO_WritePin(LED5_GPIO_Port, LED5_Pin, 0); // Transmit
}

/*
* Fsine = FtimerRTGO/Ns,   Fsine = F(timer trigger ouput)/(number of samples)
* Vsine(x)=(sine(x*(2PI/ns)+1)*((0xFFF+1)/2), this is an adjusted formula to create a positive sine.
*/

void Get_channel_1_sine(void){
	// This function generates the look up table for the sinewave signal to be generated


	for(int i=0;i<Ns;i++){
		Channel_1_sine_val[i] = ((sin(i*2*PI/Ns)+1)*((Res)/2)); // Sampling step = 2PI/ns
		Channel_1_sine_val[i] = sine_dc_offset + Channel_1_sine_scale*Channel_1_sine_val[i];
	}
}

void Get_channel_2_sine(void){
	// This function generates the look up table for the sinewave signal to be generated

	for(int i=0;i<Ns;i++){
		Channel_2_sine_val[i] = ((sin(i*2*PI/Ns)+1)*((Res)/2)); // Sampling step = 2PI/ns
		Channel_2_sine_val[i] = sine_dc_offset + Channel_2_sine_scale*Channel_2_sine_val[i];
	}
}

void set_clock_TIM2(void){
	  // This function sets up the clock to be used for the signal generation

	  HAL_TIM_Base_Stop(&htim2);

	  PSC= (Fclock/Ns)/(Freq_Signal_1*(Period + 1) ) - 1;
	  htim2.Instance = TIM2;
	  htim2.Init.Period = Period; //+1
	  htim2.Init.Prescaler = PSC; //+1 // If this value is < 50 things start to behave funny.
	  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  HAL_TIM_Base_Start(&htim2);
}

void set_clock_TIM4(void){
      // This function sets up the clock to be used for the signal generation

	  HAL_TIM_Base_Stop(&htim4);

	  PSC= (Fclock/Ns)/(Freq_Signal_2*(Period + 1) ) - 1;
	  htim4.Instance = TIM4;
	  htim4.Init.Period = Period; //+1
	  htim4.Init.Prescaler = PSC; //+1 // If this value is < 50 things start to behave funny.
	  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_TIM_Base_Start(&htim4);
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
