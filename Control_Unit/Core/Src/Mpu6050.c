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

Mpu650Com mMpu650Com;
Mpu650Value mMpu650Value;
Mpu650 mMpu6050;


void ConfigofAccelandGyro(Mpu650Value adress,Mpu650Value data )
{
	//for Accell
	mMpu650Com.Transmit[0]= adress;
	mMpu650Com.Transmit[1]= data; //

	HAL_I2C_Master_Transmit(&hi2c1, Mpu650Address, (uint8_t*)mMpu650Com.Transmit, 2, 250);
}

void InitMpu6050(void)
{
	ConfigofAccelandGyro(GyroAddress,GyroData);
	ConfigofAccelandGyro(AccellAddress,AccellData);
}



void ReadMpu6050AccellandGyro(Mpu650Value adress,char *type)
{
	//for Gyro
	mMpu650Com.Transmit[0]= adress;
	mMpu650Com.Transmit[1]= 0x00; //
	if(strcmp(type,"Transmit")==0)
	{
		HAL_I2C_Master_Transmit_IT(&hi2c1, Mpu650Address, (uint8_t*)mMpu650Com.Transmit, 1);
		mI2Case++;
	}
	if(strcmp(type,"Receive")==0)
	{
		HAL_I2C_Master_Receive_IT(&hi2c1, Mpu650Address,(uint8_t*)mMpu650Com.Receive , 2);
		mI2Case++;
	}
}


void Mpu6050Read(void)
{

	switch (mI2Case)
	{
		case Mpu6050AccellTransmitX:
			ReadMpu6050AccellandGyro(AccellXoutMsb,"Transmit");
			break;

		case Mpu6050AccellReceiveX:
			ReadMpu6050AccellandGyro(AccellXoutMsb,"Receive");
			break;

		case Mpu6050AccellTransmitY:
			ReadMpu6050AccellandGyro(AccellXoutMsb,"Transmit");
			break;

		case Mpu6050AccellReceiveY:
			ReadMpu6050AccellandGyro(AccellXoutMsb,"Receive");
			break;

		case Mpu6050AccellTransmitZ:
			ReadMpu6050AccellandGyro(AccellXoutMsb,"Transmit");
			break;

		case Mpu6050AccellReceiveZ:
			ReadMpu6050AccellandGyro(AccellXoutMsb,"Receive");
			break;

		case Mpu6050GyroTransmitX:
			ReadMpu6050AccellandGyro(GyroAddress,"Transmit");
			break;

		case Mpu6050GyroReceiveX:
			ReadMpu6050AccellandGyro(GyroAddress,"Receive");

		case Mpu6050GyroTransmitY:
			ReadMpu6050AccellandGyro(GyroAddress,"Transmit");
			break;

		case Mpu6050GyroReceiveY:
			ReadMpu6050AccellandGyro(GyroAddress,"Receive");

		case Mpu6050GyroTransmitZ:
			ReadMpu6050AccellandGyro(GyroAddress,"Transmit");
			break;

		case Mpu6050GyroReceiveZ:
			ReadMpu6050AccellandGyro(GyroAddress,"Receive");

		case Mpu6050CompletionProcess:
			mI2Case=1;
			break;
	}

}



