/*
 * Bme280.c
 *
 *  Created on: Oct 20, 2024
 *      Author: KMLCN
 */


#include <Bme280.h>
#include <stdint.h>
#include "main.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"


Raw Raw_Temp;
Bme280 mBme280;
Calibration chamber1;
Calibration chamber2;
Calibration chamber3;
Calibration chamber4;
uint8_t state=1;
uint8_t pos=1;
uint32_t holdingtimeBme280=0;
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
void Bme280_Init(uint16_t num, Calibration *mCalibm)
{
	pos=1;

	HAL_GPIO_WritePin(GPIOC,num,GPIO_PIN_RESET);
	while (pos<6)
	{
		switch(pos)
			{
				case 1:

					mBme280.Transmit[0]= 0x74; 	// ctr_meas (0xF4)
					mBme280.Transmit[1]= 0x57;	//	 010 | 101 | 11
					mBme280.Transmit[2]= 0x75; 	// config (0xF5)
					mBme280.Transmit[3]= 0x16;	//	000 | 100 | 0 | 0
					HAL_SPI_Transmit(&hspi2,mBme280.Transmit,sizeof(mBme280.Transmit),200);
					pos++;
					break;

				case 2:
					pos++;
					HAL_SPI_Transmit_IT(&hspi2, &Calib_Bme280[0],sizeof(Calib_Bme280[0]));
					break;

				case 5:
					HAL_GPIO_WritePin(GPIOC,num,GPIO_PIN_SET);
					Bme280_Const_Val_Op(mCalibm);
					pos++;
					break;
			}
	}

}

void Bme280_Raw_Temp(uint16_t row, double *temp,Calibration *chamber )
{
	 if(pos==6 && (xTaskGetTickCount() - holdingtimeBme280>= pdMS_TO_TICKS(PeriodicTemperatureTimeforBme280)))
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
					HAL_GPIO_WritePin(GPIOC,row,GPIO_PIN_SET);
			//		osDelay(500);
					*temp=Bme280_Temp(chamber);
					state=1;
					holdingtimeBme280=xTaskGetTickCount();
					break;


			}
		 }

}

void Bme280_Const_Val_Op(Calibration *mcalib)
{

	mcalib->dig_T1=	mBme280.Receive[1]<<8 | mBme280.Receive[0];
	mcalib->dig_T2=	mBme280.Receive[3]<<8 | mBme280.Receive[2];
	mcalib->dig_T3=	mBme280.Receive[5]<<8 | mBme280.Receive[4];
	mcalib->dig_P1=	mBme280.Receive[7]<<8 | mBme280.Receive[6];
	mcalib->dig_P2=	mBme280.Receive[9]<<8 | mBme280.Receive[8];
	mcalib->dig_P3=	mBme280.Receive[11]<<8 | mBme280.Receive[10];
	mcalib->dig_P4=	mBme280.Receive[13]<<8 | mBme280.Receive[12];
	mcalib->dig_P5=	mBme280.Receive[15]<<8 | mBme280.Receive[14];
	mcalib->dig_P6=	mBme280.Receive[17]<<8 | mBme280.Receive[16];
	mcalib->dig_P7=	mBme280.Receive[19]<<8 | mBme280.Receive[18];
	mcalib->dig_P8=	mBme280.Receive[21]<<8 | mBme280.Receive[20];
	mcalib->dig_P9=	mBme280.Receive[23]<<8 | mBme280.Receive[22];
}

double Bme280_Temp(Calibration *Calib)
{
	double var1, var2, T, t_fine;

	var1 = ((Raw_Temp.adc) / 16384.0 - ((double)Calib->dig_T1) / 1024.0) * ((double)Calib->dig_T2);
	var2 = (((Raw_Temp.adc) / 131072.0 - ((double)Calib->dig_T1) / 8192.0) *
	        ((Raw_Temp.adc) / 131072.0 - ((double)Calib->dig_T1) / 8192.0)) * ((double)Calib->dig_T3);
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
	HAL_SPI_Transmit_DMA(&hspi2, data,sizeof(*data));

}


