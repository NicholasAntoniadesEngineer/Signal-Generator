/*
 * UART_Commands.h
 *
 *  Created on: Jun 25, 2020
 *      Author: Nicholas Antoniades
 */

#ifndef INC_UART_COMMANDS_H_
#define INC_UART_COMMANDS_H_
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


void Motor_Send_State(UART_HandleTypeDef *huart, uint8_t tx_buff[],uint8_t uartSize, uint8_t state,uint8_t VentMethod, uint8_t BPM, uint8_t TidalVolume, uint8_t IEratio);


#endif /* INC_U1_COMMANDS_H_ */
