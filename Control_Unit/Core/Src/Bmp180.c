/*
 * Bmp180.cpp
 *
 *  Created on: Aug 14, 2025
 *      Author: Kemal
 */

#include <Bmp180.h>
#include <Sht31.h>
#include "i2c.h"
#include "main.h"
#include <Mpu6050.h>
#include <math.h>
#include "FreeRTOS.h"
#include "task.h"


 Bmp180 mBmp180;
 Bmp180Com mBmp180Com;
 Bmp180Const mBmp180Const;

 uint32_t holdingtimeBmp180=0;
 uint32_t SeaLevelPressure=101325; // Pa;
void Bmp180Init()
{
	ReadCalibrationData();
	GetCalibData(&mBmp180,&mBmp180Com);

}

void ReadCalibrationData()
{
	HAL_I2C_Mem_Read(&hi2c1, DevAdress, CalibFirstAdress, I2C_MEMADD_SIZE_8BIT,mBmp180Com.ReadCalib, 22,500);
}

void GetCalibData(Bmp180 *Data ,Bmp180Com *Read)
{
	Data->AC1=(Read->ReadCalib[0]<<8 ) | (Read->ReadCalib[1]);
	Data->AC2=(Read->ReadCalib[2]<<8 ) | (Read->ReadCalib[3]);
	Data->AC3=(Read->ReadCalib[4]<<8 ) | (Read->ReadCalib[5]);
	Data->AC4=(Read->ReadCalib[6]<<8 ) | (Read->ReadCalib[7]);
	Data->AC5=(Read->ReadCalib[8]<<8 ) | (Read->ReadCalib[9]);
	Data->AC6=(Read->ReadCalib[10]<<8) | (Read->ReadCalib[11]);
	Data->B1=(Read->ReadCalib[12]<<8 ) | (Read->ReadCalib[13]);
	Data->B2=(Read->ReadCalib[14]<<8 ) | (Read->ReadCalib[15]);
	Data->MB=(Read->ReadCalib[16]<<8 ) | (Read->ReadCalib[17]);
	Data->MC=(Read->ReadCalib[18]<<8 ) | (Read->ReadCalib[19]);
	Data->MD=(Read->ReadCalib[20]<<8 ) | (Read->ReadCalib[21]);
}

void WriteBmp180ControlData(uint8_t DataType)
{
	mBmp180Com.Write[0]=DataType;

	HAL_I2C_Mem_Write_IT(&hi2c1, DevAdress, (uint16_t)PressAndTempWriteAdress, I2C_MEMADD_SIZE_8BIT,mBmp180Com.Write,2);
	mI2Case++;
}

void ReadBmp180Register(uint16_t Adress,uint16_t size)
{
	vTaskDelay(pdMS_TO_TICKS(10)); // Reading Time
	HAL_I2C_Mem_Read_IT(&hi2c1, DevAdress, Adress, I2C_MEMADD_SIZE_8BIT,mBmp180Com.Read,size);
	mI2Case++;
}

void ReadBmp180PressureData()
{
	if(xTaskGetTickCount()- holdingtimeBmp180>= pdMS_TO_TICKS(PeriodicPressureTime))
		{
			switch (mI2Case)
			{

				case Bmp180TemperatureWrite :
					WriteBmp180ControlData(TemperatureControlData);
					break;
				case Bmp180TemperatureRead :
					ReadBmp180Register(PressAndTempReadAdress,2);
					break;
				case Bmp180CalculateTrueTemp:
					CalculateTrueTemperature();
					break;
				case Bmp180PressureWrite:
					WriteBmp180ControlData(PressureControlData);
					break;
				case Bmp180PressureRead :
					ReadBmp180Register(PressAndTempReadAdress,3);
					break;
				case Bmp180CompletionProcess :
					holdingtimeBmp180=xTaskGetTickCount();
					CalculateTruePressure();
					mI2Case=1;
					break;
			}

		}
}

void CalculateTruePressure()
{
	Bmp180 *PressData=&mBmp180;

	PressData->B6=PressData->B5 - 4000;
	PressData->X1=(PressData->B2 * (pow((PressData->B6),2) / pow(2,12))) / pow(2,11);
	PressData->X2=PressData->AC2 * PressData->B6 / pow(2,11);
	PressData->X3=PressData->X1 + PressData->X2;
	PressData->B3=((((int64_t)PressData->AC1 * 4 + PressData->X3) << Oss) + 2) / 4;
	PressData->X1=PressData->AC3 * PressData->B6 / pow(2,13);
	PressData->X2=(PressData->B1 * (pow((PressData->B6),2) / pow(2,12))) / pow(2,16);
	PressData->X3=((PressData->X1 + PressData->X2) + 2) / 4;
	PressData->B4=PressData->AC4 * (uint64_t)(PressData->X3 + 32768) / pow(2,15);
	PressData->B7=((uint64_t)PressData->RawPressure - PressData->B3) * (50000 >> Oss);

	 if(PressData->B7<0x80000000)
	 {
		 PressData->Pressure=(PressData->B7 * 2) / PressData->B4;
	 }
	 else
	 {
		 PressData->Pressure=(PressData->B7 / PressData->B4) * 2;
	 }

	 PressData->X1=(PressData->Pressure / pow(2,8)) * (PressData->Pressure / pow(2,8));
	 PressData->X1 = (PressData->X1 * 3038) / 65535;
	 PressData->X2 = (-7357 * PressData->Pressure) / 65535;
	 PressData->Pressure=PressData->Pressure + (PressData->X1 + PressData->X2 + 3791) / pow(2,4);

	 CalculateAltitute();
}

void CalculateAltitute()
{
	Bmp180 *PressureData=&mBmp180;

	PressureData->Altitute = 44330.0 * (1.0 - pow(((double)PressureData->Pressure / SeaLevelPressure), (double)(1.0 / 5.255)));

}
void CalculateTrueTemperature()
{
	Bmp180 *TempData=&mBmp180;

	TempData->X1=(TempData->RawTemperature - TempData->AC6) * TempData->AC5 / pow(2,15);
	TempData->X2=(TempData->MC * pow(2,11)) / (TempData->X1 + TempData->MD);
	TempData->B5=TempData->X1 + TempData->X2;
	TempData->Temperature=(TempData->B5 + 8 ) / 16;
	TempData->Temperature=TempData->Temperature/10;
	mI2Case++;
}

