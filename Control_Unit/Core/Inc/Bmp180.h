/*
 * Bmp180.h
 *
 *  Created on: Aug 14, 2025
 *      Author: Kemal
 */

#ifndef INC_BMP180_H_
#define INC_BMP180_H_

#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

typedef enum _Bme180Const{
	DevAdress= 0xEF<1,
	CalibFirstAdress=0xAA,
	PressureAndTempAdress=0xF6,
	PressureControlData=0x34,
	PressureControlRegister=0xF4,

}Bme180Const;


typedef struct _Bme180Com{

	uint8_t Read[3];
	uint8_t Write[2];
	uint8_t ReadCalib[22];
}Bme180Com;


typedef struct _Bme180{
uint16_t AC1;
uint16_t AC2;
uint16_t AC3;
uint16_t AC4;
uint16_t AC5;
uint16_t AC6;
uint16_t B1;
uint16_t B2;
uint16_t MB;
uint16_t MC;
uint16_t MD;
uint8_t PressMSB;
uint8_t PressLSB;
uint8_t PressXLSB;
uint32_t RawPressure;
}Bme180;


extern Bme180 mBme180;
extern Bme180Com mBme180Com;
extern Bme180Const mBme180Const;

void Bmp180Init();
void ReadCalibrationData();
void GetCalibData(Bme180 *Data ,Bme180Com *Read);
void WriteBmp180ControlData(uint8_t DataType);
void ReadBmp180Register();
void ReadBmp180PressureData();

#endif /* INC_BMP180_H_ */
