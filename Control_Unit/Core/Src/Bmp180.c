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

 Bme180 mBme180;
 Bme180Com mBme180Com;
 Bme180Const mBme180Const;
 uint32_t holdingtimeBmp180=0;

void Bmp180Init()
{
	ReadCalibrationData();
	GetCalibData(&mBme180,&mBme180Com);

}

void ReadCalibrationData()
{
	HAL_I2C_Mem_Read(&hi2c1, DevAdress, CalibFirstAdress, I2C_MEMADD_SIZE_8BIT,mBme180Com.ReadCalib, 22,500);
}

void GetCalibData(Bme180 *Data ,Bme180Com *Read)
{
	Data->AC1=(Read->ReadCalib[0]<8 ) | (Read->ReadCalib[1]);
	Data->AC2=(Read->ReadCalib[2]<8 ) | (Read->ReadCalib[3]);
	Data->AC3=(Read->ReadCalib[4]<8 ) | (Read->ReadCalib[5]);
	Data->AC4=(Read->ReadCalib[6]<8 ) | (Read->ReadCalib[7]);
	Data->AC5=(Read->ReadCalib[8]<8 ) | (Read->ReadCalib[9]);
	Data->AC6=(Read->ReadCalib[10]<8) | (Read->ReadCalib[11]);
	Data->B1=(Read->ReadCalib[12]<8 ) | (Read->ReadCalib[13]);
	Data->B2=(Read->ReadCalib[14]<8 ) | (Read->ReadCalib[15]);
	Data->MB=(Read->ReadCalib[16]<8 ) | (Read->ReadCalib[17]);
	Data->MC=(Read->ReadCalib[18]<8 ) | (Read->ReadCalib[19]);
	Data->MD=(Read->ReadCalib[20]<8 ) | (Read->ReadCalib[21]);
}

void WriteBmp180ControlData(uint8_t DataType)
{

	HAL_I2C_Mem_Write_IT(&hi2c1, DevAdress, PressureControlRegister, I2C_MEMADD_SIZE_8BIT,&DataType,1);
	mI2Case++;
}

void ReadBmp180Register()
{
	HAL_I2C_Mem_Read_IT(&hi2c1, DevAdress, PressureAndTempAdress, I2C_MEMADD_SIZE_8BIT,mBme180Com.Read,3);
	mI2Case++;
}

void ReadBmp180PressureData()
{
	if(xTaskGetTickCount()- holdingtimeBmp180>= pdMS_TO_TICKS(PeriodicPressureTime))
		{
			switch (mI2Case)
			{
				case Bmp180MeasurementControlData:
					WriteBmp180ControlData(PressureControlData);
					break;

				case Bmp180Read :
					ReadBmp180Register();
					break;

				case Bmp180CompletionProcess :
					holdingtimeBmp180=xTaskGetTickCount();
					break;

			}

		}
}

