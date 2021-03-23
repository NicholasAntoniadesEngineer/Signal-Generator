/*
 * UART_Commands.c
 *
 *  Created on: Jun 25, 2020
 *      Author: Nicholas Antoniades
 */

#include "main.h"

//----------
// u2 to u1
//----------
//void Motor_Send_State(UART_HandleTypeDef *huart, uint8_t tx_buff[],uint8_t uartSize, uint8_t state,uint8_t VentMethod, uint8_t BPM, uint8_t TidalVolume, uint8_t IEratio){
//	tx_buff[0] = state;
//	tx_buff[1] = VentMethod;
//	tx_buff[2] = BPM;
//	tx_buff[3] = TidalVolume;
//	tx_buff[4] = IEratio;
//	HAL_UART_Transmit_DMA(huart, tx_buff, uartSize);
//}
