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


typedef enum _Bmp180Const{
	DevAdress= 0x77<<1, 		//  adress
	Oss=1, 						//Controls the oversampling ratio ( (00b: single, 01b: 2 times, 10b: 4 times, 11b: 8 times) )
	CalibFirstAdress=0xAA,
	PressAndTempWriteAdress=0xF4,
	PressAndTempReadAdress=0xF6,
	TemperatureControlData=0x2E,
	PressureControlData=0x34 | (Oss << 6),


}Bmp180Const;


typedef struct _Bmp180Com{

	uint8_t Read[3];
	uint8_t Write[1];
	uint8_t ReadCalib[22];
}Bmp180Com;


typedef struct _Bmp180{
int16_t AC1;
int16_t AC2;
int16_t AC3;
uint16_t AC4;
uint16_t AC5;
uint16_t AC6;
int16_t B1;
int16_t B2;
int32_t B3;
uint32_t B4;
int32_t B5;
int32_t B6;
uint32_t B7;
int32_t X1;
int32_t X2;
int32_t X3;
int16_t MB;
int16_t MC;
int16_t MD;
uint8_t  PressMSB;
uint8_t  PressLSB;
uint8_t  PressXLSB;
uint8_t  TempMSB;
uint8_t  TempLSB;
int32_t RawPressure;
int32_t RawTemperature;
int32_t Pressure;
double Altitute;
float Temperature;
}Bmp180;

extern Bmp180 mBmp180;
extern Bmp180Com mBmp180Com;
extern Bmp180Const mBmp180Const;

void Bmp180Init();
void ReadBmp180PressureData();
void ReadCalibrationData();
void GetCalibData(Bmp180 *Data ,Bmp180Com *Read);
void WriteBmp180ControlData(uint8_t DataType);
void ReadBmp180Register(uint16_t Adress,uint16_t size);
void CalculateTruePressure();
void CalculateTrueTemperature();
void CalculateAltitute();

#endif /* INC_BMP180_H_ */
