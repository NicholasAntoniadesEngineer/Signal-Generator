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
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdio.h>
#include "sfm3000.h"
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
// Hardware defined variables
#define  gearRatio 1.6	 	      	   // Gear ratio
#define  microStep 16 		 	       // Stepper motor driver micro stepping factor
uint16_t TotalStepsPerCycle = 200;     // Set this to total steps for full rotation of the stepper motor for 1:1 stepping
uint8_t  defaultDirection = 0;	       // Used to indicate direction default of stepper driver

// Variables to be initialized for ventilation
#define  clockFreq 180000000	           // Set this to the clock frequency
#define  counterPeriod 100		       // Set the PWM counter period0
uint32_t StepsHalfCycle;		       // Set this to total steps for half rotation
uint32_t RequiredStepsHalfCycle;       // Steps required for desired tidal volume
uint32_t InspFreq;				       // Inspiration frequency
uint32_t ExpFreq;				       // Expiration frequency
uint32_t InspTime;				       // Inspiration time
uint32_t ExpTime;				       // Expiration time
uint32_t TotalCycleTime;	           // TotalCycleTime = InspTime + ExpTime
uint32_t ExpPSC;			           // Expiration PSC value
uint32_t InspPSC;			    	   // Inspiration PSC value

// Variables to be controlled for ventilation
// *** These are hard coded for now.
uint32_t VentMethod;			       // Set this to control ventilation type
uint8_t  BPM = 20;		    	       // Change this to control BPM
double 	 IEratio = 2;		           // IEratio =InspTime*ExpTime
uint8_t  Vmax = 100;			       // Max deliverable volume
uint8_t  Vdes = 100;			       // Desired volume to deliver
uint32_t stepCounter = 0;		       // Step counter
uint8_t  cycleStage = 1;               // Set initial cycle stage to Expiration
uint8_t  updateIndicator = 0;          // Used to indicate when a state update is required
int      state = 0;				       // Initialize to state 0


// Set Symbolic names: TMCL motor driver
int motorAddress = 1;   // Motor number default 1
int motorNumber = 0;	// Motor number default 0
int Bank0 = 0;			// Memory bank 0
int Bank1 = 1;			// Memory bank 1
int Bank2 = 2;			// Memory bank 2
// Set Symbolic names: Memory variables
int TMCLstate = 0;			// TMCL state
int vInsp = 1; 			// Inspiration velocity
int vExsp = 2;			// Expiration velocity
int amax = 3;			// Max acceleration
int cmax = 4;        	// Max current 0..256
int dmax = 5;			// Max deceleration
int posInspiration = 6;	// Inhaltion position
int posExpiration = 7;	// Exhalation position
// Set Symbolic names: Command variables
int GAP = 6;
int SGP = 9;			// Set Global Parameter
int GGP = 10;
int STGP = 11;			// Store Global Paramater

// Variables to be initialized for UART: HMI
#define HMIBufferSize 12			  		 // UART buffer size
uint8_t HMI_tx_buff[HMIBufferSize] = {0};	 // UART Transmit buffer
uint8_t HMI_rx_buff[HMIBufferSize] = {0};	 // UART Receive buffer

// Variables to be initialized for UART: TMCL driver
#define TMCLtxBufferSize 9			   		 // UART tx buffer size
#define TMCLrxBufferSize 9
uint8_t TMCL_tx_buff[TMCLtxBufferSize] = {0};  // UART Transmit buffer
uint8_t TMCL_rx_buff[TMCLrxBufferSize] = {0};  // UART Receive buffer

// Variables to be initialized for I2C
#define I2CBufferSize 6			   			 // I2C buffer size
uint8_t I2C_tx_buff[I2CBufferSize] = {0};	 // I2C Transmit buffer
uint8_t I2C_rx_buff[I2CBufferSize] = {0};	 // I2C Receive buffer

// Variables to be initialized for I2C: Sensirion flow sensor
float    FlowSensirion[20] = {0};			// Flow values
float	 TidalVolume[2] = {0};			// Calculated volume
uint32_t StatusSensirion = 0;			// Status bit for the sensor
uint8_t  FlowBuffer[3] = {0};			// Buffer to recieve sensor values
float FlowSensirionTotal = 0;
float FlowSensirionAverage = 0;
float deltaFlow = 0;
float deltaVolume;
float deltaFlowTime;

// Variables to be initialized for I2C: Honeywell pressure sensor
uint8_t  PressureHoneywell = 0;			// Pressure value
uint8_t  TempHoneywell = 0;				// Temperature value
uint8_t  StatusHoneywell = 0;			// Status bit for the sensor
uint8_t	 PressureBuffer[4] = {0};		// Buffer to recieve the sensor values

// Variables to be initialized for I2C: Azotech sensors
//uint8_t Device_Addr= 0x44 << 1 ;       // 8-bit device address shifted 1 for r/w bit
//uint8_t Device_write_reg = 0x09;
//uint8_t Device_read_reg = 0x02;

// Variables to initialized for ADC
#define  numADCchannels 1 		       // Set this value to be the number of channels
uint32_t value_adc[numADCchannels];    // Buffer size, 32 bit for 12 bit ADC resolution
float pressure;						   // Variable to hold the ADC value for pressure

// Variables to be initialized for timer interrupts
uint8_t toggleValue;

// Variables required for functions
// Convert to 8-bit
uint8_t eightBitResult[4];

// Variables required for RTC
RTC_DateTypeDef currentDate;
RTC_TimeTypeDef timeStampCurrent;
float timeCurrent = 0;
RTC_TimeTypeDef timeStampOld;
float timeOld = 0;
RTC_TimeTypeDef changeTime;
float deltaTime = 0;
float Seconds = 0;
//serial comms
uint8_t buf[12];
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_I2C3_Init();
  MX_DAC_Init();
  MX_RTC_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  initializations();		// User initializations
  //I2C_write(Device_Addr, Device_write_reg, I2C_tx_buff, I2C_rx_buff); // Initialize device being communicated too

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  while (1)
  {
		// Reading in from Sensirion flow sensor
		SensirionFlowSensor(FlowBuffer, FlowSensirion /*lpm*/, deltaFlow/*lpm*/,FlowSensirionTotal);

		// Getting delta time stamp from RTC
		HAL_RTC_GetTime(&hrtc, &timeStampCurrent, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
		
		// timeStampCurrent.miliSeconds is fraction of a second counting down from 255 to 0.
		Seconds = (float)(timeStampCurrent.SecondFraction-timeStampCurrent.SubSeconds) / (float)(timeStampCurrent.SecondFraction + 1);			// convert subseconds to millisecond
		timeCurrent = (float)timeStampCurrent.Hours*3600 + (float)timeStampCurrent.Minutes*60 + (float)timeStampCurrent.Seconds + Seconds;  // Current time = Hours*3600 minutes*60 + seconds + milli seconds
		deltaTime = timeCurrent - timeOld; 	// Calculating time since last sample
		timeOld = timeCurrent;			   // Set previous value = current

		//Trapeziodal integration method
		IntegrationForVolume(deltaFlow/*lpm*/, deltaTime /*s*/, FlowSensirion/*lpm*/, TidalVolume/*l*/);

		// Reading in from Honeywell pressure sensor
		HoneyWellPressure(PressureBuffer, StatusHoneywell, PressureHoneywell,TempHoneywell);

		// Toggling DMA streams for messaging and reading ADC values
		if(toggleValue == 1){
			HAL_ADC_Stop_DMA(&hadc1); 		// Close ADC DMA stream
			UARTSendDMA(eightBitResult, pressure, HMI_tx_buff, FlowSensirion, TidalVolume, HMI_rx_buff);
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
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
void initializations(void){
	// Calculating total steps to be taken by factoring in gear ratio and micro stepping.
	TotalStepsPerCycle = TotalStepsPerCycle*microStep*gearRatio;
	StepsHalfCycle = TotalStepsPerCycle/2;
	RequiredStepsHalfCycle = (TotalStepsPerCycle*Vdes)/(Vmax*2);

	// Setting up Inspiration and Expiration times, frequencies and the IE ratio.
	TotalCycleTime = 60/BPM;
	InspTime = TotalCycleTime*(IEratio/(1+IEratio));
	ExpTime  = TotalCycleTime*(1/(1+IEratio));
	InspFreq = RequiredStepsHalfCycle/InspTime;
	ExpFreq  = RequiredStepsHalfCycle/ExpTime;
	ExpPSC   = ((float)clockFreq/counterPeriod)/ExpFreq;
	InspPSC  = ((float)clockFreq/counterPeriod)/InspFreq;

	// Initializing pinch valves
	HAL_GPIO_WritePin(Pinch1_GPIO_Port, Pinch1_Pin, 0);
	HAL_GPIO_WritePin(Pinch2_GPIO_Port, Pinch2_Pin, 0);
	HAL_GPIO_WritePin(Pinch3_GPIO_Port, Pinch3_Pin, 0);
	// Power up flow sensor
	HAL_GPIO_WritePin(FlowPower_GPIO_Port, FlowPower_Pin, GPIO_PIN_SET);

	// Initializing I2C
	// Sensirion Flow sensor
	sfm3000initFlow();

	// Initialize RS485 to tx
	HAL_GPIO_WritePin(UART_RSE_GPIO_Port,UART_RSE_Pin, GPIO_PIN_SET); // Toggle RSE pin to send message over RS485

	// Initialize interrupts
//	HAL_TIM_Base_Start_IT(&htim3);
}
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin){
	switch(GPIO_Pin){

	  // TMCl OUT0 interrupt line
	  case TMCL_Stage_Pin:
		  if(HAL_GPIO_ReadPin(TMCL_Stage_GPIO_Port, TMCL_Stage_Pin))
		  {
			  	HAL_GPIO_WritePin(Pinch1_GPIO_Port, Pinch1_Pin, 0);
			  	HAL_GPIO_WritePin(Pinch2_GPIO_Port, Pinch2_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(Pinch3_GPIO_Port, Pinch3_Pin, GPIO_PIN_SET);

		  }else{

			  	HAL_GPIO_WritePin(Pinch1_GPIO_Port, Pinch1_Pin, GPIO_PIN_SET);
			  	// This delay needs to be done correctly
				for(int i = 4000000;i>0;i--){
					__NOP();
				}
				HAL_GPIO_WritePin(Pinch2_GPIO_Port, Pinch2_Pin, 0);
				HAL_GPIO_WritePin(Pinch3_GPIO_Port, Pinch3_Pin, 0);


		  }
		  TidalVolume[0] = 0;
		  TidalVolume[1] = 0;
		break;

	  // B1 button interrupt
	  case B1_Pin:
		  switch(state){
			  case 0:
				  while(TMCL_rx_buff[8] != 1){
					  TMCL_UART_Message(motorAddress,SGP,TMCLstate,Bank2,1);
				  }
				  state = 1;
				  break;

			  case 1:
				  while(TMCL_rx_buff[8] != 0){
				   TMCL_UART_Message(motorAddress,SGP,TMCLstate,Bank2,0);
				  }
				  state = 0;
				  break;
		  }
		  break;
	}
}
void IntegrationForVolume(float deltaFlow, float deltaTime,	float FlowSensirion[20], float TidalVolume[2]) {
	// Trapeziodal integration method
	// volume(t=1)"L"   = volume(t=0)"L" + (deltaTime)"min"              *(deltaFlow)"lpm"
	// volume(t=1)"mil" = volume(t=0)"L" + (deltaTime)"sec" *(1/60)"min" *(deltaFlow)"lpm" *(1000)"litres"

	TidalVolume[0] = TidalVolume[1] + (deltaTime /*s*/ *0.01666)/*min*/*((FlowSensirion[1] - FlowSensirion[0])*(0.5) + FlowSensirion[1])/*l*/*1000/*mil*/;   // Tidal Volume in L
	TidalVolume[1] = TidalVolume[0];
}

void TMCL_UART_Message(uint8_t moduleAddress,uint8_t commandNumber,uint8_t commandType,uint8_t motorNumber,uint32_t commandValue){

	// Breaking a 32 bit value into 8 byte values.
	uint32_t ValueByte0 = ((commandValue >> 24) & 0xFF) ;
	uint32_t ValueByte1 = ((commandValue >> 16) & 0xFF) ;
	uint32_t ValueByte2 = ((commandValue >> 8 ) & 0XFF);
	uint32_t ValueByte3 = (commandValue & 0XFF);

	// TMCL Command Format
	TMCL_tx_buff[0] = moduleAddress;          // Module address
	TMCL_tx_buff[1] = commandNumber; 	      // Command number
	TMCL_tx_buff[2] = commandType;            // Type number
	TMCL_tx_buff[3] = motorNumber; 		      // Motor or bank number
	TMCL_tx_buff[4] = ValueByte0; 	          // Value byte 1 (MSB first)
	TMCL_tx_buff[5] = ValueByte1;		      // Value byte 2
	TMCL_tx_buff[6] = ValueByte2;      		  // Value byte 3
	TMCL_tx_buff[7] = ValueByte3;        	  // Value byte 4
	TMCL_tx_buff[8] = CheckSum(TMCL_tx_buff); // Checksum

	HAL_UART_Transmit(&huart2, TMCL_tx_buff, TMCLtxBufferSize,1); 	// Send message
	for(int i = 1000;i>0;i--){
		__NOP();
	}
	HAL_GPIO_WritePin(UART_RSE_GPIO_Port,UART_RSE_Pin , 0);		    // Toggle RSE pin to recieve message over RS485
	HAL_UART_Receive(&huart2, TMCL_rx_buff, TMCLrxBufferSize,1);   // Receive reply
	for(int i = 16000;i>0;i--){
		__NOP();
	}
	HAL_GPIO_WritePin(UART_RSE_GPIO_Port,UART_RSE_Pin , 1);		    // Toggle RSE pin to recieve message over RS485

	// TMCL Reply Format
	//		TMCL_rx_buff[0] = ;	// reply address
	//		TMCL_rx_buff[1] = ; // module address
	//		TMCL_rx_buff[2] = ; // status
	//		TMCL_rx_buff[3] = ; // command number
	//		TMCL_rx_buff[4] = ; // Value byte 1 (MSB first)
	//		TMCL_rx_buff[5] = ; // Value byte 2
	//		TMCL_rx_buff[6] = ; // Value byte 3
	//		TMCL_rx_buff[7] = ; // Value byte 4
	//      TMCL_rx_buff[8] = CheckSum;
}
void SensirionFlowSensor(uint8_t FlowBuffer[3], float FlowSensirion[20], float deltaFlow, float FlowSensirionTotal) {
	// Reading in from Sensirion flow sensor
	HAL_I2C_Master_Receive(SFM3000_I2C_INS, SFM3000_I2C_ADDRESS, FlowBuffer,
			SFM3000_READ_DATA_SIZE, 0xFF);
	// Creating an array with previous flow values
	for (int i = 7; i > 0; i--) {
		FlowSensirion[i] = FlowSensirion[i - 1];
	}
	// Updating flow latest value
	FlowSensirion[0] = ((((float) ((FlowBuffer[0] << 8) | FlowBuffer[1])) - SFM3000_OFFSET_PARAMETER ) / SFM3000_SCALE_PARAMETER ); // Calculate flow from sensor output
	// Check for large change in Flow possibly caused by noise.
	deltaFlow = FlowSensirion[1] - FlowSensirion[0]; //Calculate change in flow
//	if (deltaFlow > 100 || deltaFlow < -100) {
//		// Flow at this point is in slm.
//		FlowSensirion[0] = FlowSensirion[1]; // If it is let current flow val = previous flow val
//	}
	// Check to see if this is the standard method.
	// Check to see if flow sensor requires reset
	if (FlowSensirion[0] == FlowSensirion[1]) {
		// Initialize sensirion flow sensor
		sfm3000initFlow();
	}
	// Rolling average filter
//	FlowSensirionTotal = FlowSensirion[0] + FlowSensirion[1];
//	FlowSensirion[0] = FlowSensirionTotal / 2;

	//		int filterValue = 2;
	//		for(int i = 2; i>= 0; i ++){
	//			FlowSensirionTotal = FlowSensirionTotal + FlowSensirion[i];
	//		}
}
void UARTSendDMA(uint8_t eightBitResult[4], float pressure,	uint8_t HMI_tx_buff[HMIBufferSize], float FlowSensirion[20],float TidalVolume[2], uint8_t HMI_rx_buff[HMIBufferSize]) {
	// Pressure value for comms
	CTBVTFEBV(eightBitResult, (uint16_t) pressure);
	HMI_tx_buff[0] = eightBitResult[0];
	HMI_tx_buff[1] = eightBitResult[1];
	if (FlowSensirion[0] < 0) {
		HMI_tx_buff[2] = (-1) * FlowSensirion[0];
		HMI_tx_buff[3] = 1;
	} else {
		HMI_tx_buff[2] = FlowSensirion[0];
		HMI_tx_buff[3] = 0;
	}
	// Volume for comms
	CTBVTFEBV(eightBitResult, (uint16_t) TidalVolume[0]);
	HMI_tx_buff[4] = eightBitResult[0];
	HMI_tx_buff[5] = eightBitResult[1];
	HMI_tx_buff[11] = 255; // Good message check
	HAL_UART_Transmit_DMA(&huart1, HMI_tx_buff, HMIBufferSize); // Send
	HAL_UART_Receive_DMA(&huart1, HMI_rx_buff, HMIBufferSize); // Receive
}
void CTBVTFEBV(uint8_t *eightBitresult,uint32_t value){  //Convert a 32 bit value to 4 bytes
	uint8_t eightBitpart1 = 0;
	uint8_t eightBitpart2 = 0;
	uint8_t eightBitpart3 = 0;
	uint8_t eightBitpart4 = 0;
	int counter = 0;
    int digit;

	while (value > 0) {
	 digit = value % 10;
	 switch(counter){
		 case 0:
			 eightBitpart1 = digit;
	 	 	 break;
		 case 1:
			 eightBitpart1 = eightBitpart1 + digit*10 ;
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

	if(huart == &huart2){
	  //HAL_GPIO_WritePin(UART_RSE_GPIO_Port,UART_RSE_Pin, 0); // Toggle RSE pin to send message over RS485
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//HAL_UART_Receive_DMA(&huart1, HMI_rx_buff, HMIBufferSize);

	// Messages recieved from TMCL
	if(huart == &huart2){
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
void I2C_read(uint8_t Device_Addr, uint16_t Device_reg, uint8_t I2C_tx_buff[], uint8_t I2C_rx_buff[]){
	  // Counter to avoid infinite while loop
	  int counter = 0;
	  int counterLimit = 100;
	  // I2C_tx_buff[0] = Device_reg;
	  // I2C_tx_buff[0] = byteCommand[0];
	  // I2C_tx_buff[1] = byteCommand[1];

	  if (HAL_I2C_Master_Transmit(&hi2c3, Device_Addr, I2C_tx_buff, 1, 100) != HAL_OK){
		  __NOP();
	  }else{
		  while(1){
			  // Receive information
			  if (HAL_I2C_Master_Receive(&hi2c3, Device_Addr, I2C_rx_buff,4, 100) != HAL_OK){
					 if(counter >= counterLimit){
						 counter ++;
					 }else{
						 counter = 0;
						 break;
					 }
			  }else{
//				  sprintf((char*)I2C_rx_buff,"%u %u %u %u %u \r\n", ((unsigned int)I2C_rx_buff[0]),((unsigned int)I2C_rx_buff[1]),((unsigned int)I2C_rx_buff[2]) ,((unsigned int)I2C_rx_buff[3]) ,((unsigned int)I2C_rx_buff[4]));
//				  HAL_UART_Transmit(&huart2,I2C_rx_buff,strlen((char*)I2C_rx_buff), 100);
				  break;
			  }
		  }
	  }
}
void I2C_write(uint8_t Device_Addr, uint16_t Device_reg, uint8_t I2C_tx_buff[], uint8_t I2C_rx_buff[]){
	// Counter to avoid infinite while loop
	int counter = 0;
	int counterLimit = 100;
	while(1){
	  // I2C_tx_buff[0] = Device_reg;
	  // I2C_tx_buff[0] = byteCommand[0];
	  // I2C_tx_buff[1] = byteCommand[1];

	  if (HAL_I2C_Master_Transmit(&hi2c3, Device_Addr, I2C_tx_buff, 1, 100) != HAL_OK){
			 if(counter >= counterLimit){
				 counter ++;
			 }else{
				 counter = 0;
				 break;
			 }
	  }else{
		  counter = 0;
		  while(1){
			  if ( HAL_I2C_Mem_Write(&hi2c3, Device_Addr, Device_reg,1,I2C_tx_buff,5, 100) != HAL_OK){
					 if(counter >= counterLimit){
						 counter ++;
					 }else{
						 counter = 0;
						 break;
					 }
			  }else{
//				  sprintf((char*)I2C_rx_buff,"Write complete \r\n");
//				  HAL_UART_Transmit(&huart2,I2C_rx_buff,strlen((char*)I2C_rx_buff), 100);
				  __NOP();
				  break;
			  }
		  }
		  break;
	   }
	}
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
