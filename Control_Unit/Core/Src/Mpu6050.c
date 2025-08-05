/*
 * Mpu6050.c
 *
 *  Created on: Jun 11, 2025
 *      Author: Kemal
 */


#include "i2c.h"
#include <Sht31.h>
#include <Mpu6050.h>
#include <string.h>
#include <stdlib.h>
Mpu650Com mMpu650Com;
Mpu650Value mMpu650Value;
Mpu650 mMpu6050;


void ConfigofAccelandGyro(Mpu650Value adress,Mpu650Value data )
{

	mMpu650Com.Transmit[0]= adress;
	mMpu650Com.Transmit[1]= data; //

	HAL_I2C_Master_Transmit(&hi2c1,(uint16_t) Mpu650Address, (uint8_t*)mMpu650Com.Transmit, 2, 250);
}

void InitMpu6050(void)
{
	ConfigofAccelandGyro(SignalPathResetAdress,SignalPathResetData);
	ConfigofAccelandGyro(Mpu6050ConfigAdress,Mpu6050ConfigData);
	ConfigofAccelandGyro(GyroAddress,GyroConfigData);
	ConfigofAccelandGyro(AccellAddress,AccellConfigData);
	ConfigofAccelandGyro(PowerManagement1Adress,PowerManagement1Data);
	ConfigofAccelandGyro(PowerManagement2Adress,PowerManagement2Data);
	ConfigofAccelandGyro(SampleRateDividerAdress,SampleRateDividerData);

	mI2Case=6;
}



void ReadMpu6050AccellandGyro(Mpu650Value AdressH)
{
	 mI2Case++;
	 HAL_I2C_Mem_Read_IT(&hi2c1,(uint16_t) Mpu650Address, (uint16_t) AdressH, (uint16_t) I2C_MEMADD_SIZE_8BIT, (uint8_t*)mMpu650Com.Receive, 2);

}


void Mpu6050Read(void)
{

	switch (mI2Case)
	{

		case Mpu6050AccellReadX:
			ReadMpu6050AccellandGyro(AccellXoutH);
			break;

		case Mpu6050AccellReadY:
			ReadMpu6050AccellandGyro(AccellYoutH);
			break;

		case Mpu6050AccellReadZ:
			ReadMpu6050AccellandGyro(AccellZoutH);
			break;

		case Mpu6050GyroReadX:
			ReadMpu6050AccellandGyro(GyroXoutH);
			break;

		case Mpu6050GyroReadY:
			ReadMpu6050AccellandGyro(GyroYoutH);
			break;

		case Mpu6050GyroReadZ:
			ReadMpu6050AccellandGyro(GyroZoutH);
			break;

		case Mpu6050CompletionProcess:
			mI2Case=6;
			break;
	}

}

void TransferReceiverDataforMpu6050(char *in , uint8_t *out)
{
	*(out) = (uint8_t)*(in);
	 *(out + 1) = (uint8_t)*(in + 1);
}



