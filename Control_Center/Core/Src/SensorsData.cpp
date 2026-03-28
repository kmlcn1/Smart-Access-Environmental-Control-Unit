/*
 * SensorsData.cpp
 *
 *  Created on: Mar 23, 2026
 *      Author: Kemal
 */

#include <SensorsData.h>
#include <Lcd.h>
#include "stdio.h"
#include "math.h"
#include "Nrf2401Plus.h"

mSensorsData SensorsData;


void mSensorsData :: ShowAllData()
{
if(mLcd.HoldingTime - PerioadicLcdTime >0)
	{
		 char line1[21]="";
		 char line2[21]="";
		 char line3[21]="";
		 char line4[21]="";



		if(SensorsData.Get.ParseFlag==true && SensorsData.Get.NrfReceive==true)
		{
			snprintf(line1,sizeof(line1),"ST:%.2f Alti:%.2f",SensorsData.Get.SecTemp,SensorsData.Get.Altitude);
			snprintf(line2,sizeof(line2),"RT:%.2f",SensorsData.Get.RoomTemp);
			snprintf(line3,sizeof(line3),"X:%.1f Y:%.1f",SensorsData.Get.GX,SensorsData.Get.GY);
			snprintf(line4,sizeof(line4),"Z:%.1f TO:%d",SensorsData.Get.GZ,SensorsData.Get.NrfTimeout);
			mLcd.Print(	line1,
						line2,
						line3,
						line4);
		}
		else if(SensorsData.Get.NrfReceive==false && NrfTimeOut > (SensorsData.Get.NrfTimeout + 1000) )
		 {
			mLcd.Print(	"   ",
						"  RF Communication ",
						"  Failure ",
						"   ");
		 }
		else if(SensorsData.Get.ParseFlag==false)
		{
			mLcd.Print(	"   "			,
						"   RF Data "	,
						"   Missing  "	,
						"   ");
		}
	}
}
void mSensorsData:: PressToHeight(int Press)
{
	if(SensorsData.Get.Press!=0)
	SensorsData.Get.Altitude=(SensorsData.Const.T0 / SensorsData.Const.L)
							*(1.0 - pow((double)(SensorsData.Get.Press / SensorsData.Const.P0),
							(double)((SensorsData.Const.GasConstant * SensorsData.Const.L)
							/(double)(SensorsData.Const.MolarMass * SensorsData.Const.g))));

	else
		SensorsData.Get.Altitude=-66;

}
