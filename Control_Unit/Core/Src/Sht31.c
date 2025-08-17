/*
 * Sht31.c
 *
 *  Created on: Jun 7, 2025
 *      Author: Kemal
 */


#include <Sht31.h>
#include "i2c.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

I2C mI2C;
SHT31Clock mSHT31Clock;
Sht31 mSht31;
I2Case mI2Case=0;
uint32_t holdingtimeSht31=0;
uint8_t I2CAdress=0;

void Sht31_Temperature(void)
{

	if(xTaskGetTickCount()- holdingtimeSht31>= pdMS_TO_TICKS(PeriodicTemperatureTimeforSth31))
	{
		// AD pin is connected to the ground.
		Sht31_Enable_Measurement_with_Clock_Streching(Enable,EnableLow);
	}
	else
	{
//		mI2Case=8;
	}
}

void Sht31_Enable_Measurement_with_Clock_Streching(uint8_t situation,uint8_t Repeatability)
{

	mI2C.Transmit[0]=situation;
	mI2C.Transmit[1]=Repeatability;


	switch (mI2Case)
	{
		case Sht31Transmit:
			HAL_I2C_Master_Transmit_IT(&hi2c1,(uint16_t) Sht31Adress,mI2C.Transmit , 2);
			mI2Case++;
			break;

		case Sht31Receive:
			HAL_I2C_Master_Receive_IT(&hi2c1, Sht31Adress,mI2C.Receive , 3);
			mI2Case++;
			break;
		case Sht31CompletionProcess:
			mI2Case++;
			break;
	}

}



uint8_t CRC_Calculator(const uint8_t *data)
{
    uint8_t crc = 0xFF;  // Initial Value
    for (uint8_t i = 0; i < 2; i++)
    {
        crc ^= data[i];
        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x80) // Last bit is 1?
            {
                crc = (crc << 1) ^ Sht31CRCPol;
            }
            else
            {
                crc <<= 1;
            }
        }

    }
    return crc;
}

void TemperatureConversion(void)
{
	mSht31.RawTemp=mI2C.Receive[0]<<8 | mI2C.Receive[1];

	mSht31.TempCelcius= -45+(175*mSht31.RawTemp)/( pow(2, 16)-1);

}


