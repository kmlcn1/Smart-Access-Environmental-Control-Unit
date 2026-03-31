/*
 * Rtc.cpp
 *
 *  Created on: Mar 30, 2026
 *      Author: Kemal
 */

#include <Rtc.h>
#include "i2c.h"
#include "main.h"
#include <SensorsData.h>
#include "stdint.h"

mRtc Rtc;



void mRtc :: Init()
{

	Rtc.Com.TransmitInit[0]= 50	;			// second
	Rtc.Com.TransmitInit[1]= 37	;			// minute
	Rtc.Com.TransmitInit[2]= 20	;			// hours
	Rtc.Com.TransmitInit[3]= 0	;			// Day
	Rtc.Com.TransmitInit[4]= 31	;			// Date
	Rtc.Com.TransmitInit[5]= 3	;			// Month
	Rtc.Com.TransmitInit[6]= 25	;			// Year

	Rtc.SetBcdFormat( Rtc.Com.TransmitInit);

	HAL_I2C_Mem_Write(&hi2c1, Rtc.Adress, Rtc.SecAdress, 1, Rtc.Com.TransmitInit, 7 ,300);

}


void mRtc :: GetDate()
{
	if(Rtc.HoldingRtcTime > PeriodiRtcTime )//&& NrfReceivingDataFlag==true)
		{
			switch(SensorsData.Get.CaseNum)
			{

				case SensorsData.RtcReadData:

					SensorsData.Get.CaseNum++;
					HAL_I2C_Mem_Read_DMA(&hi2c1, Rtc.Adress, Rtc.SecAdress, 1, Rtc.Com.Receive, 7);
					break;

				case SensorsData.RtcFnishOperation :

					SensorsData.Get.CaseNum++;
					break;

			}

		}

}

void mRtc :: GetFromBcd(uint8_t *Data)
{
	for(uint8_t i=0; i<7; i++)
	{
		*(Data +i)= ((*(Data +i) >> 4 ) * 10) + (*(Data +i) & 0xF );
	}
}

void mRtc :: SetBcdFormat( uint8_t *Data)
{

	for(uint8_t  i=0; i<7; i++)
	{
		*(Data +i)= ((*(Data +i) / 10 ) << 4) | (*(Data +i) % 10);
	}

}
