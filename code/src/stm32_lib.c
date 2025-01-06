/*
 * Library.c
 * Created on: 24 Apr 2020
 * Author: Nicholas Antoniades
 */

#define STM32F051
#include "stm32f0xx.h"
#include <stdint.h>
#include "stm32_lib.h"
#include <stm32f0xx_exti.h>
#include "stm32f051x8.h"
#include "stm32_bsp.h"

void stm32_lib_port_init(port_config_t *port_config) {
	if (!port_config) return;

	// Initialize GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	for (int i = 0; i < port_config->num_pins_a; i++) {
		GPIOA->MODER &= ~(0x3 << (port_config->pins_a[i].pin * 2));
		GPIOA->MODER |= (port_config->pins_a[i].mode << (port_config->pins_a[i].pin * 2));
		GPIOA->PUPDR &= ~(0x3 << (port_config->pins_a[i].pin * 2));
		GPIOA->PUPDR |= (port_config->pins_a[i].pull << (port_config->pins_a[i].pin * 2));
	}

	// Initialize GPIOB
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	for (int i = 0; i < port_config->num_pins_b; i++) {
		GPIOB->MODER &= ~(0x3 << (port_config->pins_b[i].pin * 2));
		GPIOB->MODER |= (port_config->pins_b[i].mode << (port_config->pins_b[i].pin * 2));
		GPIOB->PUPDR &= ~(0x3 << (port_config->pins_b[i].pin * 2));
		GPIOB->PUPDR |= (port_config->pins_b[i].pull << (port_config->pins_b[i].pin * 2));
	}

	// Call BSP or HAL init if needed
	stm32_bsp_gpio_init();
}

// Initializing the PWM output
void stm32_lib_init_pwm(int frequency)
{
	#define GPIO_AFRH_AFR11_AF2 ((uint32_t)0x00002000)		/*Macros to be defined*/
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN; 						// Enable clock for GPIOB
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;	 					// Enable TIM2
	GPIOB->MODER |= GPIO_MODER_MODER11_1;					// set PB10 to AF
	GPIOB->AFR[1] |= (GPIO_AFRH_AFR11_AF2&(0b10<<12)); 		// Enable AF2 for PB10 in GPIOB AFR10
	TIM2->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // PWM Mode 1 	// PWM Mode 1
	TIM2->ARR = frequency; 									// Setting signal frequency
	TIM2->PSC = 0;											// Setting signal prescalar
	TIM2->CCR4 = frequency/2; 								// PWM Duty cycle based on fractions of ARR
	TIM2->CCER |= TIM_CCER_CC4E; 							// Compare 3 output enable
	//TIM2->CR1 |= TIM_CR1_CEN;    //Counter enable
}

// Initializing the ADC
void stm32_lib_init_adc(void)
{
	ADC1 -> CR |= ADC_CR_ADCAL;          // ADC calibration
	RCC -> APB2ENR |= RCC_APB2ENR_ADCEN; // Enabling the ADC clock in the RCC APB  periperal clock enableregister
	ADC1 -> CFGR1 |= 0x10;               // Setting the ADC resolution in the ADC configuration register 1
	ADC1->CHSELR = 0b100000;	         // Selecting the chanel for pot-0.
	ADC1 -> CR |= ADC_CR_ADEN;           // Enabling the ADC
	while((ADC1 -> ISR & 0x01) ==0 );    // Waiting for the ADRDY pin to let us know the ADC is ready.
}

// Initializing the ADC to a specific pin
int stm32_lib_adc_input(int input, int resolution)
{
    ADC1->CFGR1 |= (resolution << 3);	/* configure resolution,12,10,8,6 bits */
    ADC1->CHSELR = (1 <<  input);		// channel select where input ranges from ADC-in 0-13
    ADC1->CR |= (0b1 << 2); 		    // start conversion
    while ((ADC1->ISR & 0b100) == 0);	// wait for conversion to end
    return ADC1->DR;					// return value
}

int stm32_lib_adc_awd_check(void)
{
	if((ADC1->ISR & 0x80) == 1)
	{	// Check if AWD status bit is high
		ADC1 -> ISR &= ~0x80;
		return 1;
	}else{
		return 0;
	}
}

void stm32_lib_adc_awd_8bit(int ADC_channel, int ADC_Low_threshhold, int ADC_High_threshhold)
{
	// ADC_Low_threshhold = (ADC_Low_threshhold*256)/3.3;
	// ADC_High_threshhold = (ADC_High_threshhold*256)/3.3;

	ADC1 -> CFGR1 |= 1 << 22;  					// Set AWDEN bit high to enable AWD
	ADC1 -> CFGR1 |= 1 << 21;					// Set AWDSGL bit high to select signle channel.
	ADC1 -> CFGR1 |= ADC_channel << 25;			// Set AWDCH[4:0] bits to select channel.
	ADC1 -> TR |= ADC_High_threshhold << 15;	// Set ADC_HTR high threshholds.
	ADC1 -> TR |= ADC_Low_threshhold;			// Set ADC_LTR low threshhold.
	ADC1 -> IER |= 0x80;						// Set AWDIE bit in ADC_IER to enable the interrupt.
}

// Fetching ADC data
int stm32_lib_adc_data(void)
{
	ADC1 ->CR |= ADC_CR_ADSTART;		// Starts ADC conversion
	while((ADC1 -> ISR & 0b100) ==0 );	// Waits for the End of conversion flag to be set
	return ADC1 -> DR;
}

// Creating a delay by iterating through a loop
void stm32_lib_delay(int time) {
	// time given in milli seconds
	time = time * 727;
	while (time > 0) {time--;}
}

// Check for button press GPIOA
int stm32_lib_check_button_gpioa(int button) {
	if ((GPIOA->IDR & (0b1 << button)) == 0) {
		stm32_lib_debounce();
		return 1;
	} else { return 0; }
}
// Check for button press GPIOB
int stm32_lib_check_button_gpiob(int button) 
{
	if ((GPIOB->IDR & (0b1 << button)) == 0) {
		stm32_lib_debounce();
		return 1;
	} else { return 0; }
}

// Check for button debouncee
void stm32_lib_debounce(void) 
{
	int x = 36350;		// 50 milliseconds
	while (x > 0) {x--;}
}

// Enabling Timer 6 interrupt
void stm32_lib_init_tim6(uint32_t arr, uint32_t psc)
{
    RCC -> APB1ENR |= (1 << 4);		// Enable Tim6 in the peripheral clock enable register
    TIM6 -> PSC = psc;
    TIM6 -> ARR = arr;
    TIM6 -> DIER |= TIM_DIER_UIE;	// Enable Tim6 interrupt
    TIM6->SR &= ~TIM_SR_UIF;		// Clear the update flag
    TIM6 -> CR1 |= TIM_CR1_CEN;		// Enable Tim6 Counter
    TIM6 -> CR1 |= TIM_CR1_ARPE;	// Enable Tim6 Auto reload / preload
    NVIC_EnableIRQ(TIM6_DAC_IRQn); 	// Enable Tim6 interrupt in the (NVIC) Nested Vectored Interrupt Controller
}

// Acknowledging timer interrupt
void stm32_lib_ack_irq(void)
{
        TIM6->SR &= ~(1 << 0);	// Set interrupt acknowledge flag to zero (not 1)
}

// Initializing the External Interrupt
void stm32_lib_init_exti(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;		// Enables the clock for the sys config controller.
	SYSCFG->EXTICR[1] |=SYSCFG_EXTICR1_EXTI1_PA;    // Map PA1 to EXTI1.
	EXTI->IMR |=EXTI_IMR_MR1; 						// Unmasks EXTI1 in interrupt mask register
	EXTI->FTSR |= EXTI_FTSR_TR1; 					// Falling trigger enable for input line 1 (SW1)
	NVIC_EnableIRQ(EXTI0_1_IRQn);					// Enable the interrupt in the NVIC
}


void stm32_lib_init_usart1(void)
{
	// Must enable usart1_DE and the associated pins AF
	// Program the M bits in USARTx_CR1 to define the word length.
	//Program the number of stop bits in USARTx_CR2.
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 				// enable clock for UART1
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; 					// enable clock to port a
	GPIOA->MODER |= GPIO_MODER_MODER9_1; 				// Set PA9 to AF mode
	GPIOA->MODER |= GPIO_MODER_MODER10_1;				// Set PA10 to AF mode
	GPIOA->AFR[1] = 0b00000000000000000000000100010000; // Map AF1 for PA9 and PA10
	USART1->CR1 &=~ USART_CR1_M; 						// clear M0(bit 12) of USARTx_CR1
	SystemCoreClockUpdate(); 							// define clock speed
	USART1->BRR = SystemCoreClock/115200;		     		// set baud rate to 115.2kbps for oversampling by 16
	USART1->CR1 &=~ USART_CR1_PCE;						// no parity
	USART1->CR2 &=~ USART_CR2_STOP;						// 1 stop bit
	USART1->CR1 |= USART_CR1_UE;						// Enable USART1
	USART1->CR1 |= USART_CR1_TE;						// Enable USART1_TE Set idle frame as first transmission.
	USART1->CR1 |= USART_CR1_RE;						// Enable USART1_RX
}

void stm32_lib_usart1_transmit(unsigned char DataToTx)
{
	// wait until TXE = 1 before writing int tdr

	USART1 -> TDR = DataToTx; 					// write character �c� to the USART1_TDR
	while((USART1 -> ISR & USART_ISR_TC) == 0); // wait: transmission complete
}

unsigned char stm32_lib_usart1_receive(void)
{
	unsigned char DataRx = 'z';					  // global
	while((USART1 -> ISR & USART_ISR_RXNE) == 0); // exits loop when data is received
	DataRx = USART1 -> RDR; 					  // store received data into �DataRx� variable
	USART1 -> ICR = 0b111111111111111111111;
	return DataRx;
}

void stm32_lib_lock_crystal(void) 
{
  RCC->CR |= RCC_CR_HSEON; // enable HSE
  while(!(RCC->CR & RCC_CR_HSERDY)); // hange here until HSE ready
  // the following adds a wait state to Flash reads and enables the prefetch buffer. This may or may not be necessary...
  FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;
  RCC->CFGR |= RCC_CFGR_PLLMUL6; // PLLCLK = HSE * 6 = 8 * 6 = 48 MHz = maximum
  RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV; // select HSE as source for PLL
  RCC->CR |= RCC_CR_PLLON; // enable the PLL
  while(!(RCC->CR & RCC_CR_PLLRDY)); // hang here until PLL ready
  RCC->CFGR |= RCC_CFGR_SW_PLL; // SYSCLK sourced from PLL
  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL)); // hang until SYSLK switched
}

void stm32_lib_unlock_crystal(void) 
{
  RCC->CFGR &= ~RCC_CFGR_SW; // clear the SYSCLK selection bits, causing SYSCLK to be sourced from HSI
  while(RCC->CFGR & RCC_CFGR_SWS_PLL); // hang until SYSLK no longer PLL
  RCC->CR &= ~RCC_CR_HSEON;
}

void stm32_lib_sig_gen_init(stm32_sig_gen_state_t *state, const stm32_sig_gen_state_t *config)
{
    if (!state || !state->timer) return;

    // Copy configuration to state
    memcpy(state, config, sizeof(stm32_sig_gen_state_t));

    // Enable timer clock
    if (state->timer == TIM2)
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    else if (state->timer == TIM4)
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // Configure timer for PWM
    state->timer->Instance->PSC = 0;
    state->timer->Instance->ARR = state->frequency;
    
    // Configure PWM mode based on channel
    switch(state->channel) {
        case TIM_CHANNEL_1:
            state->timer->Instance->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1);
            state->timer->Instance->CCR1 = (state->amplitude * state->frequency) / 100;
            state->timer->Instance->CCER |= TIM_CCER_CC1E;
            break;
        case TIM_CHANNEL_2:
            state->timer->Instance->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1);
            state->timer->Instance->CCR2 = (state->amplitude * state->frequency) / 100;
            state->timer->Instance->CCER |= TIM_CCER_CC2E;
            break;
        case TIM_CHANNEL_3:
            state->timer->Instance->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1);
            state->timer->Instance->CCR3 = (state->amplitude * state->frequency) / 100;
            state->timer->Instance->CCER |= TIM_CCER_CC3E;
            break;
        case TIM_CHANNEL_4:
            state->timer->Instance->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1);
            state->timer->Instance->CCR4 = (state->amplitude * state->frequency) / 100;
            state->timer->Instance->CCER |= TIM_CCER_CC4E;
            break;
    }

    // Enable timer
    state->timer->Instance->CR1 |= TIM_CR1_CEN;
}

void stm32_lib_pwm(void)
{
    /*Macros to be defined*/
    #define GPIO_AFRH_AFR10_AF2 ((uint32_t)0x00000200)
    #define GPIO_AFRH_AFR11_AF2 ((uint32_t)0x00002000)

    //Initialising clocks and pins for PWM output
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;     // Enable clock for GPIOB
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;    // Enable TIM2
    GPIOB->MODER |= GPIO_MODER_MODER10_1; // set PB10 to AF
    GPIOB->MODER |= GPIO_MODER_MODER11_1; // set PB11 to AF

    //Choosing AF for pins, MAPPING them to TIM2 CH3 and CH4
    GPIOB->AFR[1] |= (GPIO_AFRH_AFR10_AF2&(0b10<<8)); //Enable AF2 for PB10 in GPIOB AFR10
    GPIOB->AFR[1] |= (GPIO_AFRH_AFR11_AF2&(0b10<<12));//Enable AF2 for PB11 in GPIOB AFR11

    // specify PWM mode: OCxM bits in CCMRx. We want mode 1
    TIM2->CCMR2 |= (TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1); // PWM Mode 1
    TIM2->CCMR2 |= (TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1); // PWM Mode 1

    //Setting signal frequency
    TIM2->ARR = 48000; // f = 1 KHz
    TIM2->PSC = 0;

    //PWM Duty cycle based on fractions of ARR
    TIM2->CCR3 = 0 * 480; // Red = 20%
    TIM2->CCR4 = 20 * 480; // Green = 90%

    // Enable output compare for CH3 and CH4
    TIM2->CCER |= TIM_CCER_CC3E; //Compare 3 output enable
    TIM2->CCER |= TIM_CCER_CC4E; //Compare 4 output enable
    TIM2->CR1 |= TIM_CR1_CEN;    //Counter enable
}

void stm32_lib_uart_init(stm32_uart_state_t *uart_state, const stm32_uart_state_t *config) {
    if (!uart_state || !config) return;

    // Copy configuration to state
    memcpy(uart_state, config, sizeof(stm32_uart_state_t));

    // Initialize UART hardware
    stm32_bsp_uart_init(uart_state->huart, (uint32_t)uart_state->huart->Instance, uart_state->huart->Init.BaudRate);
}
