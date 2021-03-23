/*
------------------------------------------------------------------------------
   Project	: Ventilator control system
   File     : sfm3000.c
   Brief    : Source file for SFM3000 Mass Flow Sensor.
   Author 	: Nicholas Antoniades
------------------------------------------------------------------------------

Device address values
- 0x40 The default I2C address.
- 0x81 The default I2C header for read access.

Command register values
- 0x1000 Start flow measurement, move internal pointer to flow measurement result register
- 0x1001 Start temp measurement, move internal pointer to temp measurement result register
- 0x31AE Read Serial Number (bit 31:0)
- 0x30DE Read scale factor
- 0x30DF Read offset
- 0x2000 Soft reset command
 */

#include "sfm3000.h"

//  Initiates the data reading for flow.
uint8_t sfm3000initFlow(void){
	// Two byte command to start continuous measurement.
	uint8_t Buffer[SFM3000_DEVICE_I2C_COMMAND_SIZE];
	Buffer[0] = READ_FLOW_DATA_BYTE_1;
	Buffer[1] = READ_FLOW_DATA_BYTE_2;
	return (uint8_t) I2CwriteBytes(SFM3000_I2C_ADDRESS, Buffer, SFM3000_DEVICE_I2C_COMMAND_SIZE);
}

//  Initiates the data reading for temperature.
uint8_t sfm3000initTemp(void){
	// Two byte command to start continuous measurement.
	uint8_t Buffer[SFM3000_DEVICE_I2C_COMMAND_SIZE];
	Buffer[0] = READ_TEMP_DATA_BYTE_1;
	Buffer[1] = READ_TEMP_DATA_BYTE_2;
	return (uint8_t) I2CwriteBytes(SFM3000_I2C_ADDRESS, Buffer, SFM3000_DEVICE_I2C_COMMAND_SIZE);
}


// Reads in the data.
float sfm3000getValue(float offset, float scale, uint8_t *Status){
	uint8_t Buffer[3] = {0};
	*Status = (uint8_t) HAL_I2C_Master_Receive(SFM3000_I2C_INS, SFM3000_I2C_ADDRESS, Buffer, SFM3000_READ_DATA_SIZE, 0xFF);

	// Cyclic redundancy checking (CRC) is a technique used for error detection in data transmission.
	#ifdef CRC_ON
		if(HAL_OK == sfm3000crcCheck(Buffer[0], Buffer[1], Buffer[2]))
			return ((((float) ((Buffer[0] << 8) | Buffer[1])) - offset) / scale);
		else
			return HAL_ERROR;
	#else
		return ((((float) ((Buffer[0] << 8) | Buffer[1])) - offset) / scale);
	#endif
}

// Initiates soft reset to the sensor, sends the two byte command to reset.
uint8_t sfm3000reset(void){
	uint8_t Buffer[SFM3000_DEVICE_I2C_COMMAND_SIZE];
	Buffer[0] = SOFT_RESET_BYTE_1;
	Buffer[1] = SOFT_RESET_BYTE_2;
	return (uint8_t) I2CwriteBytes(SFM3000_I2C_ADDRESS, Buffer, SFM3000_DEVICE_I2C_COMMAND_SIZE);
}

// Reads in the serial number from the sensor
uint32_t sfm3000readSerialDevId(void){
	uint8_t Buffer[SFM3000_SERIAL_ID_SIZE];
	Buffer[0] = READ_SERIAL_ID_BYTE_1;
	Buffer[1] = READ_SERIAL_ID_BYTE_2;
	if(HAL_OK == I2CwriteBytes(SFM3000_I2C_ADDRESS, Buffer, SFM3000_DEVICE_I2C_COMMAND_SIZE)){
		HAL_Delay(5);
		I2CreadBytes(SFM3000_I2C_ADDRESS, Buffer, SFM3000_SERIAL_ID_SIZE);
		#ifdef CRC_ON
				if(HAL_OK == (sfm3000crcCheck(Buffer[0], Buffer[1], Buffer[2]) & sfm3000crcCheck(Buffer[3], Buffer[4], Buffer[5])))
					return ((Buffer[0] << 24) | (Buffer[1] << 16) | (Buffer[3] << 8) | (Buffer[4]));
				else
					return HAL_ERROR;
		#else
				return ((Buffer[0] << 24) | (Buffer[1] << 16) | (Buffer[3] << 8) | (Buffer[4]));
		#endif
	}
	return HAL_ERROR;
}


// Calculates the CRC byte with respect to input.
uint8_t sfm3000crc8(const uint8_t data, uint8_t crc){
	crc ^= data;
	for (uint8_t i = 8; i; --i )
		crc = ( crc & 0x80 ) ? (crc << 1) ^ 0x31 : (crc << 1);
	return crc;
}


// Checks the response from the sensor with respect to CRC.
uint8_t sfm3000crcCheck(uint8_t in1, uint8_t in2, uint8_t crcRead){
	uint8_t crcTemp = 0xFF;
	crcTemp = sfm3000crc8(in1, crcTemp);
	if(crcRead == sfm3000crc8(in2, crcTemp))
		return HAL_OK;
	else
		return HAL_ERROR;
}


// Library can be used with different I2C libraries by only changing these functions without changing the SFM3000 related functions.
uint8_t I2CwriteBytes(uint16_t devAddr, uint8_t * buffer, uint8_t size){
	return (uint8_t) HAL_I2C_Master_Transmit(SFM3000_I2C_INS, devAddr, buffer, size, 0xFF);
}
uint8_t I2CreadBytes(uint16_t devAddr, uint8_t * buffer, uint8_t size){
	return (uint8_t) HAL_I2C_Master_Receive(SFM3000_I2C_INS, devAddr, buffer, size, 0xFF);
}
