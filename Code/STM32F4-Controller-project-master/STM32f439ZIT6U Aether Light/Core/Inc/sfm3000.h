/*
------------------------------------------------------------------------------
   Project	: Ventilator control system
   File     : sfm3000.h
   Brief    : Header file for SFM3000 Mass Flow Sensor.
   Author 	: Nicholas Antoniades
------------------------------------------------------------------------------
*  Instance is defined as &hi2c2 and can be changed to a different I2C_HandleTypeDef. However,
*  I2CreadBytes and  I2CwriteBytes functions must be updated in case of different architectures.
*
*  sfm3000init function activates the reading data from the sensor, so initiating the function once,
*  then reading by sfm3000getValue function at each time is the general procedure. After power-up
*  or reset, sfm3000init function must be executed again.
*
*  To activate CRC check of the responses, comment out the related definition.
*
*  Detailed tutorial can be found at (http://embeddedarea.com/sensirion-sfm3000-mass-flow-meter-example/)
*
*/



#ifndef SFM3000_H_
#define SFM3000_H_
#include <stdint.h>
#include "i2c.h"   // Related HAL Library for I2C Peripheral.

#define SFM3000_I2C_INS				    &hi2c1 			// Address of I2C HAL Instance.
#define SFM3000_I2C_ADDRESS 			0x40<<1			// Device I2C Address.
#define TRANSMISSON_TIMEOUT_PARAMETER 	0xFF			// Timeout parameter of write and read functions.
#define SFM3000_OFFSET_PARAMETER		(float) 32000.0 // Offset flow value from the data sheet.
#define SFM3000_SCALE_PARAMETER 		(float) 140.0   // Scale factor flow value of air from the data sheet.

//#define CRC_ON //Comment out to enable CRC on receive

enum SFM3000_COMMAND_SIZES {
	SFM3000_DEVICE_I2C_COMMAND_SIZE = 0x02, 			// Size of commands
	SFM3000_SERIAL_ID_SIZE      	= 0x06, 			// 4 byte id + 2 byte CRC info.
	SFM3000_READ_DATA_SIZE          = 0x03 				// 2 byte data + 1 byte CRC info.
};

// Command values
//- 0x1000 Start flow measurement, move internal pointer to flow measurement result register
//- 0x1001 Start temp measurement, move internal pointer to temp measurement result register
//- 0x31AE Read Serial Number (bit 31:0)
//- 0x30DE Read scale factor
//- 0x30DF Read offset
//- 0x2000 Soft reset command


enum SFM3000_COMMANDS {
	READ_TEMP_DATA_BYTE_1 = 0x10,
	READ_TEMP_DATA_BYTE_2 = 0x01,

	READ_FLOW_DATA_BYTE_1 = 0x10,
	READ_FLOW_DATA_BYTE_2 = 0x00,
	READ_SERIAL_ID_BYTE_1 = 0x31,
	READ_SERIAL_ID_BYTE_2 = 0xAE,
	SOFT_RESET_BYTE_1     = 0x20,
	SOFT_RESET_BYTE_2     = 0x00
};

uint8_t  sfm3000initFlow(void);
uint8_t  sfm3000initTemp(void);
float    sfm3000getValue(float offset, float scale, uint8_t * status);
uint8_t  sfm3000reset(void);
uint32_t sfm3000readSerialDevId(void);
uint8_t  sfm3000crc8(const uint8_t data, uint8_t crc);
uint8_t  sfm3000crcCheck(uint8_t in1, uint8_t in2, uint8_t crcRead);
uint8_t  I2CreadBytes(uint16_t devAddr, uint8_t * buffer, uint8_t size);
uint8_t  I2CwriteBytes(uint16_t devAddr, uint8_t * buffer, uint8_t size);


#endif /* SFM3000_H_ */
