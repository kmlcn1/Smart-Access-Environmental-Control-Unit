/*
 * Sht31.h
 *
 *  Created on: Jun 7, 2025
 *      Author: Kemal
 */


#ifndef ST31_h_
#define ST31_h_
#include <stdint.h>



#define 	Sht31Adress 							0x44<<1
#define 	Sht31CRCPol								0x31
typedef struct _I2C{
	uint8_t Transmit[2];
	uint8_t Receive[3];
}I2C;

typedef enum _SHT31Clock{

	Enable=0x2C,
	Disable=0x24,
	EnableHigh=0x06,
	EnableMedium=0x0D,
	EnableLow=0x10,
	DisableHigh=0x00,
	DisableMedium=0x0B,
	DisableLow=0x16,

}SHT31Clock;

typedef struct _Sht31{
	uint16_t RawTemp;
	float TempCelcius;
}Sht31;

typedef enum _I2Case{
	Sht31Transmit=1,
	Sht31Receive=3,
	Sht31CompletionProcess=5,
	Mpu6050AccellReadX	=6,
	Mpu6050AccellReadY	=8,
	Mpu6050AccellReadZ	=10,
	Mpu6050GyroReadX	=12,
	Mpu6050GyroReadY	=14,
	Mpu6050GyroReadZ	=16,
	Mpu6050CompletionProcess=18,
	Bmp180MeasurementControlData=19,
	Bmp180Read=21,
	Bmp180CompletionProcess=23,
}I2Case;


extern SHT31Clock mSHT31Clock;
extern Sht31 mSht31;
extern I2C mI2C;
extern I2Case mI2Case;
extern uint16_t holdingtime;
extern uint8_t I2CAdress;
extern uint32_t holdingtimeSht31;

void Sht31_Temperature(void);
void Sht31_Enable_Measurement_with_Clock_Streching(uint8_t situation,uint8_t Repeatability);
uint8_t CRC_Calculator(const uint8_t *data);
void TemperatureConversion(void);

#endif
