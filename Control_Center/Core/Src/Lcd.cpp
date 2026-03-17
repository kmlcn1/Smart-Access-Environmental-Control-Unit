/*
 * Lcd.cpp
 *
 *  Created on: Mar 10, 2026
 *      Author: Kemal
 */

#include <Lcd.h>
#include <string>
#include "string.h"
#include "stdio.h"

Lcd mLcd;

void Lcd::EnPulseInit(uint8_t data , Lcd :: ConstLcd endis)
{
	uint8_t buffer;
	buffer = data & endis ;
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
}

void Lcd::EnPulse(uint8_t data , Lcd :: ConstLcd endis)
{
	uint8_t buffer;
	buffer = data & endis ;
	HAL_I2C_Master_Transmit_DMA(&hi2c1, mLcd.SlaveAdress, &buffer, 1);
}

void Lcd :: Transmit(uint8_t *LcdTransmitData)
{
	mLcd.mI2CLcd.CaseNum++;
	HAL_I2C_Master_Transmit_DMA(&hi2c1, mLcd.SlaveAdress, LcdTransmitData, 1);
}

void Lcd::Print(const char *data1 ,const char *data2,
					const char *data3, const char *data4)
{


	memset(mLcd.mI2CLcd.buffer,' ',sizeof(mLcd.mI2CLcd.buffer)); 		// All buffer is space character
	memcpy(reinterpret_cast<char*> (mLcd.mI2CLcd.buffer), data1, strlen(data1));
	memcpy(reinterpret_cast<char*> (mLcd.mI2CLcd.buffer + 20) ,data3, strlen(data3));
	memcpy(reinterpret_cast<char*> (mLcd.mI2CLcd.buffer + 40) ,data2, strlen(data2));
	memcpy(reinterpret_cast<char*> (mLcd.mI2CLcd.buffer + 60) ,data4, strlen(data4));

	for(mLcd.mI2CLcd.TransmitCounter=0 ; mLcd.mI2CLcd.TransmitCounter < sizeof(mLcd.mI2CLcd.buffer);)
	{
		switch(mLcd.mI2CLcd.CaseNum)
		{
			case mLcd.HighCase :

				mLcd.mI2CLcd.Data=((mLcd.mI2CLcd.buffer[mLcd.mI2CLcd.TransmitCounter] & 0xF0) | LcdInitData);
				mLcd.Transmit(&mLcd.mI2CLcd.Data); // High Case
				break;

			case mLcd.LowCase :
				mLcd.mI2CLcd.Data=(((mLcd.mI2CLcd.buffer[mLcd.mI2CLcd.TransmitCounter] & 0xF) << 4 ) | LcdInitData);
				mLcd.Transmit(&mLcd.mI2CLcd.Data); // Low Case
				break;

		}


	}

}

void Lcd::Init()
{
	uint8_t buffer;

	buffer = Lcd8BitMode | LcdInitInstruct;
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);
	HAL_Delay(10);

	buffer = Lcd8BitMode | LcdInitInstruct;
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);
	HAL_Delay(1);

	buffer = Lcd8BitMode | LcdInitInstruct;
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = Lcd4BitMode | LcdInitInstruct;
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = (Lcd4Bit2Line  & 0xF0 ) | LcdInitInstruct; // High Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = ((Lcd4Bit2Line  & 0xF) << 4) | LcdInitInstruct;  // Low Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = (LcdDispOnCurserOff & 0xF0) | LcdInitInstruct; // High Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = ((LcdDispOnCurserOff & 0xF) << 4) | LcdInitInstruct; // low Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = (LcdClearDisp & 0xF0) | LcdInitInstruct; // High Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = ((LcdClearDisp & 0xF) << 4) | LcdInitInstruct; // Low Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = (LcdEntrySet & 0xF0) | LcdInitInstruct; // High Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);

	buffer = ((LcdEntrySet & 0xF) << 4) | LcdInitInstruct; // Low Case
	HAL_I2C_Master_Transmit(&hi2c1, mLcd.SlaveAdress, &buffer, 1,200);
	mLcd.EnPulseInit(buffer,LcdDisable);
}


void Lcd::Example()
{
	if(mLcd.HoldingTime - PerioadicLcdExampleTime >0)
	{

		mLcd.Print("        " ,
				   "   Hello" ,
				   "   World" ,
				   "        " );

	}

}


