/*
 * Bme280.c
 *
 *  Created on: Oct 20, 2024
 *      Author: KMLCN
 */


#include "../Src/Bme280.h"

#include <stdint.h>
#include "main.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"


Raw Raw_Temp;
Calibration chamber1;
Calibration chamber2;
Calibration chamber3;
Calibration chamber4;
Bme280 Calib;
uint8_t state=1;
uint8_t pos=1;
uint8_t Calib_Bme280[]={0x88,0x89,0x8A,0x8B,
                        0x8C,0x8D,0x8E,0x8F,
                        0x90,0x91,0x92,0x93,
                        0x94,0x95,0x96,0x97,
                        0x98,0x99,0x9A,0x9B,
                        0x9C,0x9D,0x9E,0x9F};

Const Adress = {.temp_msb=0xFA,	.temp_lsb=0xFB,.temp_xlsb=0xFC,
				.press_msb=0xF7,.press_lsb=0xF8,.press_xlsb=0xF9,
									.hum_msb=0xFD,	.hum_lsb=0xFE,
};
void Bme280_Init(uint16_t num, Calibration mCalibm)
{
	pos=1;
	HAL_GPIO_WritePin(GPIOC,num,GPIO_PIN_RESET);
	while (pos<9)
	{
		switch(pos)
			{
				case 1:
					pos++;
					memset(Calib.Transmit,0,sizeof(Calib.Transmit));
					Calib.Transmit[0]= 0x74; // ctr_meas (0xF4)
					Calib.Transmit[1]=0x4B;	//	 010 | 010 | 11
					Spi_Transmit(Calib.Transmit); //
					break;
				case 3:
					pos++;
					memset(Calib.Transmit,0,sizeof(Calib.Transmit));
					Calib.Transmit[0]= 0x75; // config (0xF5)
					Calib.Transmit[1]= 0x64;	//	000 | 100 | 0 | 0
					Spi_Transmit(Calib.Transmit); //
					break;
				case 5:
					pos++;
					HAL_SPI_Transmit_IT(&hspi2, &Calib_Bme280[0],sizeof(Calib_Bme280[0]));
					break;
				case 8:
					HAL_GPIO_WritePin(GPIOC,num,GPIO_PIN_SET);
					Bme280_Const_Val_Op(mCalibm);
					pos++;
					break;
			}
	}

}

void Bme280_Raw_Temp(uint16_t row, double temp)
{
		switch(state)
		{
			case 1:
				HAL_GPIO_WritePin(GPIOC,row,GPIO_PIN_RESET);
				state++;
				Spi_Transmit(&Adress.temp_msb);
				break;
			case 4:
				state++;
				Spi_Transmit(&Adress.temp_lsb);
				break;
			case 7:
				state++;
				Spi_Transmit(&Adress.temp_xlsb);

				break;
			case 10:
				state++;
				Raw_Temp_Get();
				break;
			case 11:
				temp=Bme280_Temp();
				state=1;
				HAL_GPIO_WritePin(GPIOC,row,GPIO_PIN_SET);

		//	    osDelay(500);
		}

}

void Bme280_Const_Val_Op(Calibration mcalib)
{
	mcalib.dig_T1=	Calib.Receive[0]<<8 | Calib.Receive[1];
	mcalib.dig_T2=	Calib.Receive[2]<<8 | Calib.Receive[3];
	mcalib.dig_T3=	Calib.Receive[4]<<8 | Calib.Receive[5];
	mcalib.dig_P1=	Calib.Receive[6]<<8 | Calib.Receive[7];
	mcalib.dig_P2=	Calib.Receive[8]<<8 | Calib.Receive[9];
	mcalib.dig_P3=	Calib.Receive[10]<<8 | Calib.Receive[11];
	mcalib.dig_P4=	Calib.Receive[12]<<8 | Calib.Receive[13];
	mcalib.dig_P5=	Calib.Receive[14]<<8 | Calib.Receive[15];
	mcalib.dig_P6=	Calib.Receive[16]<<8 | Calib.Receive[17];
	mcalib.dig_P7=	Calib.Receive[18]<<8 | Calib.Receive[19];
	mcalib.dig_P8=	Calib.Receive[20]<<8 | Calib.Receive[21];
	mcalib.dig_P9=	Calib.Receive[22]<<8 | Calib.Receive[23];
}

double Bme280_Temp(Calibration mCalib)
{
	double var1, var2, T, t_fine;

	var1 = ((Raw_Temp.adc) / 16384.0 - ((double)mCalib.dig_T1) / 1024.0) * ((double)mCalib.dig_T2);
	var2 = (((Raw_Temp.adc) / 131072.0 - ((double)mCalib.dig_T1) / 8192.0) *
	        ((Raw_Temp.adc) / 131072.0 - ((double)mCalib.dig_T1) / 8192.0)) * ((double)mCalib.dig_T3);
	t_fine = var1 + var2;
	T = (var1 + var2) / 5120.0;
	return T;


}
void Raw_Temp_Get()
{
	Raw_Temp.adc= Raw_Temp.temp_msb << 12 | Raw_Temp.temp_lsb << 4 | Raw_Temp.temp_xlsb >> 4;
}

void Spi_Transmit(uint8_t *data)
{
	HAL_SPI_Transmit_IT(&hspi2, data,sizeof(*data));

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if( hspi==&hspi2)
	{
		if(pos==2 || pos==4 )
		{

			pos++;
		}
		else if (pos==6)
		{
			HAL_SPI_Receive_IT(&hspi2, Calib.Receive, sizeof(Calib.Receive));
			pos++;


		}

		if(state==2)
		{
	//		HAL_SPI_Receive_IT(&hspi2,(uint8_t*)&Raw_Temp,sizeof(Raw_Temp.temp_msb)+sizeof(Raw_Temp.temp_lsb)+sizeof(Raw_Temp.temp_xlsb));
			HAL_SPI_Receive_IT(&hspi2, &Raw_Temp.temp_msb,sizeof(Raw_Temp.temp_msb));
			state++;
		}

		else if(state==5)
		{
			HAL_SPI_Receive_IT(&hspi2, &Raw_Temp.temp_lsb,sizeof(Raw_Temp.temp_lsb));
			state++;
		}

		else if(state==8)
		{
			HAL_SPI_Receive_IT(&hspi2, &Raw_Temp.temp_xlsb,sizeof(Raw_Temp.temp_xlsb));
			state++;
		}
	}
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{

	if( hspi==&hspi2)
	{
		if(pos==7)
		{
			pos++;
		}

		if(state==3)
		{
			state++;
		}

		else if(state==6)
		{
			state++;
		}

		else if(state==9)
		{
			state++;
		}

	}
}
