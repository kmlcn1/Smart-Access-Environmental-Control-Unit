/*
 * Nrf2401Plus.c
 *
 *  Created on: Dec 1, 2025
 *      Author: Kemal
 */

#include "Nrf2401Plus.h"
#include "spi.h"
#include <Bme280.h>
#include <Bmp180.h>
#include <Mpu6050.h>
#include <Sht31.h>
#include <Tmc2208.h>
#include <string.h>
#include <stdio.h>

NrfConst mNrfConst;
NrfAdress mNrfAdress;
NrfCom mNrfCom;
NrfCase mNrfCase=1;

uint32_t holdingtimeNrf=0;

void NrfInit(uint8_t Select)
{
	HAL_GPIO_WritePin(GPIOA, NRFChipEnable_Pin, GPIO_PIN_SET);	// ChipsEnable pin active
	switch(Select)
	{
				case  RX :
					HAL_GPIO_WritePin(GPIOA, NRF_Cs_Pin, GPIO_PIN_RESET);  // CsN pin low
					NrfMode(RxMode);
					NrfEnableRxPipe(EnRxPipe1);
					NrfPipe(RxPipe0,0xAABBCCDDEE);
					NrfPipe(TxPipe0,0xAABBCCDDEE);
					NrfAutoAck(DisAllAA);
					NrfSetupAdressWidth(AdressWidth5);
					NrfFreqChannel(115);
					NrfRFSetup(DataRate250kbps,RFOutputPower0dBm);
					NrfRxPayload(32);
					HAL_GPIO_WritePin(GPIOA, NRF_Cs_Pin, GPIO_PIN_SET);
					break;

				case TX :
					HAL_GPIO_WritePin(GPIOA, NRF_Cs_Pin, GPIO_PIN_RESET);
					NrfMode(TxMode);
					NrfEnableRxPipe(EnRxPipe1);
					NrfPipe(RxPipe0,0xAABBCCDDEE);
					NrfPipe(TxPipe0,0xAABBCCDDEE);
					NrfSetupAutoRetransmit(DisAutoReTransmit);
					NrfSetupAdressWidth(AdressWidth5);
					NrfFreqChannel(115);
					NrfRFSetup(DataRate250kbps,RFOutputPower0dBm);
					NrfRxPayload(32);
					HAL_GPIO_WritePin(GPIOA, NRF_Cs_Pin, GPIO_PIN_SET);
					break;
	}

}

void NrfMode(uint8_t ModeSelect)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (Config & 0x1F); 	// Write Adress with Map Register(Config)
	mNrfCom.TransmitInit[1]=ModeSelect;						// Data
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfEnableRxPipe(uint8_t Enable)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (EnRxPipe & 0x1F); 	// Write Adress with Map Register(Config)
	mNrfCom.TransmitInit[1]=Enable;						// Data
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi2, mNrfCom.TransmitInit, 3, 200);
}
void NrfPipe(uint8_t Adress , uint64_t Activate)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (Adress & 0x1F); 	// Write Adress with Map Register(Config)
	mNrfCom.TransmitInit[1]=Activate >> 32;						// Data
	mNrfCom.TransmitInit[2]=(Activate >>24 & 0xFF);
	mNrfCom.TransmitInit[3]=(Activate >>16 & 0xFF);
	mNrfCom.TransmitInit[4]=(Activate >>8 & 0xFF);
	mNrfCom.TransmitInit[5]=Activate & 0xFF;
	HAL_SPI_Transmit(&hspi2, mNrfCom.TransmitInit, 6, 200);
}

void NrfAutoAck(uint8_t Data)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (AutoAck & 0x1F);
	mNrfCom.TransmitInit[1]=Data;
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfSetupAdressWidth(uint8_t Width)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (SetupAW & 0x1F);
	mNrfCom.TransmitInit[1]=Width;
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfFreqChannel(uint8_t Channel)
{
	if(Channel>125)
		Channel=125;

	mNrfCom.TransmitInit[0]=WriteMode ^ (RfChanel & 0x1F);
	mNrfCom.TransmitInit[1]=Channel;
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfRFSetup(uint8_t DataRate , uint8_t RfOutputPower)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (RfSetup & 0x1F);
	mNrfCom.TransmitInit[1]= DataRate ^ (RfOutputPower & 0x07);
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfRxPayload(uint8_t PayloadSize)
{
	if(PayloadSize>32)
		PayloadSize=32;

	mNrfCom.TransmitInit[0]=WriteMode ^ (RxPayload & 0x1F);
	mNrfCom.TransmitInit[1]=PayloadSize;
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfSetupAutoRetransmit(uint8_t Value)
{
	mNrfCom.TransmitInit[0]=WriteMode ^ (SetupReTR & 0x1F);
	mNrfCom.TransmitInit[1]=Value;
	mNrfCom.TransmitInit[2]=0x00;
	HAL_SPI_Transmit(&hspi3, mNrfCom.TransmitInit, 3, 200);
}

void NrfDataSending()
{
	if(xTaskGetTickCount()- holdingtimeNrf>= pdMS_TO_TICKS(PeriodicNrfDataTransmitionTime))
	{
		switch(mNrfCase)
		{
			case  DataBuild1 :

				NrfDataBuild(TxPayloadNoActW,FirstSendingData);
				break;

			case  TransmitCase1 :

				NrfTransmit();
				break;

			case  DataBuild2 :

				NrfDataBuild(TxPayloadNoActW,SecondSendingData);
				break;

			case  TransmitCase2 :

				NrfTransmit();
				holdingtimeNrf=xTaskGetTickCount();
				mNrfCase=DataBuild1;
				HAL_GPIO_WritePin(GPIOA, NRF_Cs_Pin, GPIO_PIN_SET);
				break;
		}
	}

}

void NrfDataBuild(uint8_t TxType , uint8_t Selection)
{
	// "ST:21.56-P:12300-RT:23.45-DL:32"
	// "GX:2-GY:3-GZ:5-DL:20"

	char buffer[20];
	memset(mNrfCom.Transmit,0,sizeof(mNrfCom.Transmit));
	mNrfCom.Transmit[0]=TxType;
	switch(Selection)
	{
		case FirstSendingData :

			strcat((mNrfCom.Transmit),"ST:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%.2f",mBme280.temp1);
			strcat(mNrfCom.Transmit,buffer);
			strcat(mNrfCom.Transmit,"-P:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%ld",mBmp180.Pressure);
			strcat(mNrfCom.Transmit,buffer);
			strcat(mNrfCom.Transmit,"-RT:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%.2f",mSht31.TempCelcius);
			strcat(mNrfCom.Transmit,buffer);

			strcat(mNrfCom.Transmit,"-DL:");
			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d-",strlen(mNrfCom.Transmit)+3);
			strcat(mNrfCom.Transmit,buffer);

			mNrfCase++;
			break;

		case SecondSendingData :

			strcat((mNrfCom.Transmit),"GX:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d",mMpu6050.GyroX);
			strcat(mNrfCom.Transmit,buffer);
			strcat(mNrfCom.Transmit,"-GY:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d",mMpu6050.GyroY);
			strcat(mNrfCom.Transmit,buffer);
			strcat(mNrfCom.Transmit,"-GZ:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d",mMpu6050.GyroZ);
			strcat(mNrfCom.Transmit,buffer);

			strcat(mNrfCom.Transmit,"-DL:");
			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d-",strlen(mNrfCom.Transmit)+3);
			strcat(mNrfCom.Transmit,buffer);

			mNrfCase++;
			break;
	}

}

void NrfTransmit()
{
	HAL_GPIO_WritePin(GPIOA, NRF_Cs_Pin, GPIO_PIN_RESET);
	//HAL_SPI_Transmit_DMA(&hspi3, (uint8_t *) mNrfCom.Transmit, strlen(mNrfCom.Transmit));

	HAL_SPI_Transmit(&hspi3, (uint8_t *) mNrfCom.Transmit, strlen(mNrfCom.Transmit),200);
	mNrfCase++;
	mNrfCase++;
}
