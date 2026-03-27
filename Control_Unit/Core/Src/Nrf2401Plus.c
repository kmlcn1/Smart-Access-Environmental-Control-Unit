/*
 * Nrf2401Plus.c
 *
 *  Created on: Dec 1, 2025
 *      Author: Kemal
 */

#include "Nrf2401Plus.h"
#include "spi.h"
#include <string.h>
#include <stdio.h>
#include "main.h"


#if 	UsingControlCenter
#include <string>
#endif


#if UsingControlUnit
#include "cmsis_os.h"
#include <Bme280.h>
#include <Bmp180.h>
#include <Mpu6050.h>
#include <Sht31.h>
#include <Tmc2208.h>
#endif



NrfConst mNrfConst;
NrfAdress mNrfAdress;
NrfCom mNrfCom;
NrfCase mNrfCase=DataBuild1;

uint32_t holdingtimeNrf=0;
GPIO_TypeDef* NrfChipSelectPort;
uint16_t NrfChipSelectPinNum;
GPIO_TypeDef* NrfChipEnPort;
uint16_t NrfChipEnablePinNum;
SPI_HandleTypeDef *mSpi;
bool NrfReceivingDataFlag=false;

void NrfInit(uint8_t Select, GPIO_TypeDef* ChipEnPort, uint16_t ChipEnPin, GPIO_TypeDef* ChipSelectPort, uint16_t ChipSelectPin, SPI_HandleTypeDef *SpiType )
{
	mSpi=SpiType;
	NrfChipEnPort=ChipEnPort;
	NrfChipEnablePinNum=ChipEnPin;
	NrfChipSelectPort=ChipSelectPort;
	NrfChipSelectPinNum	=ChipSelectPin;

	HAL_GPIO_WritePin(NrfChipEnPort, NrfChipEnablePinNum, GPIO_PIN_RESET);	// ChipEnable pin DeActive

	switch(Select)
	{
				case  RX :

					NrfMode(RxMode);
					NrfEnableRxPipe(EnAAP0);
					NrfPipe(RxPipe0Adress,RxPipeV);
					NrfPipe(TxPipe0Adress,TxPipeV);
					NrfAutoAck(EnRxPipe1);
					NrfSetupAdressWidth(AdressWidth5);
					NrfFreqChannel(ChNum);
					NrfRFSetup(DataRate250kbps,RFOutputPower0dBm);
					NrfRxPayload(32);
					HAL_GPIO_WritePin(NrfChipEnPort, NrfChipEnablePinNum, GPIO_PIN_SET);
					break;

				case TX :

					NrfEnableRxPipe(EnAAP0);
					NrfPipe(RxPipe0Adress,RxPipeV);
					NrfPipe(TxPipe0Adress,TxPipeV);
					NrfAutoAck(EnRxPipe1);
					NrfSetupAutoRetransmit(uS500,ReTransmitCount8);
					NrfSetupAdressWidth(AdressWidth5);
					NrfFreqChannel(ChNum);
					NrfRFSetup(DataRate250kbps,RFOutputPower0dBm);
					NrfMode(TxMode);
					NrfRxPayload(32);
					break;
	}

}

void NrfMode(uint8_t ModeSelect)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (Config & 0x1F); 	// Write Adress with Map Register(Config)
	mNrfCom.TransmitInit[1]=ModeSelect;						// Data

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfEnableRxPipe(uint8_t Enable)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (EnRxPipe & 0x1F);
	mNrfCom.TransmitInit[1]=Enable;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}
void NrfPipe(uint8_t Adress1 , uint64_t Activate)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCom.TransmitInit[0]=WriteMode | (Adress1 & 0x1F); 	// Write Adress with Map Register(Config)
	mNrfCom.TransmitInit[1]=Activate >> 32;						// Data
	mNrfCom.TransmitInit[2]=((Activate >>24) & 0xFF);
	mNrfCom.TransmitInit[3]=((Activate >>16) & 0xFF);
	mNrfCom.TransmitInit[4]=((Activate >>8) & 0xFF);
	mNrfCom.TransmitInit[5]=Activate & 0xFF;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 6, 500);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfAutoAck(uint8_t Data)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (AutoAck & 0x1F);
	mNrfCom.TransmitInit[1]=Data;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfSetupAdressWidth(uint8_t Width)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (SetupAW & 0x1F);
	mNrfCom.TransmitInit[1]=Width;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfFreqChannel(uint8_t Channel)
{

	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	if(Channel>125)
		Channel=125;

	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (RfChanel & 0x1F);
	mNrfCom.TransmitInit[1]=Channel;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfRFSetup(uint8_t DataRate , uint8_t RfOutputPower)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (RfSetup & 0x1F);
	mNrfCom.TransmitInit[1]= DataRate | (RfOutputPower & 0x07);

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfRxPayload(uint8_t PayloadSize)
{
	if(PayloadSize>32)
		PayloadSize=32;
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (RxPayload & 0x1F);
	mNrfCom.TransmitInit[1]=PayloadSize;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfSetupAutoRetransmit(uint8_t Delay, uint8_t ReTransCount)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (SetupReTR & 0x1F);
	mNrfCom.TransmitInit[1]=((Delay & 0xF) << 4 ) | (ReTransCount & 0xF) ;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfSetupFeature(uint8_t Value)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	memset(mNrfCom.TransmitInit,0,sizeof(mNrfCom.TransmitInit));
	mNrfCom.TransmitInit[0]=WriteMode | (Feature & 0x1F);
	mNrfCom.TransmitInit[1]=Value;

	HAL_SPI_Transmit(mSpi, mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfClearAct()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCom.TransmitInit[0]=WriteMode | (Status & 0x1F);
	mNrfCom.TransmitInit[1]=ClearAct;

	HAL_SPI_Transmit(mSpi, (uint8_t *) mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfClearReTransmitCount()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCom.TransmitInit[0]=WriteMode | (Status & 0x1F);
	mNrfCom.TransmitInit[1]=ClearReTransmit;

	HAL_SPI_Transmit(mSpi, (uint8_t *) mNrfCom.TransmitInit, 2, 200);
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_SET);
}

void NrfDataSending()
{
	if(xTaskGetTickCount()- holdingtimeNrf>= pdMS_TO_TICKS(PeriodicNrfDataTransmitionTime))
	{
		switch(mNrfCase)
		{
			case DataBuild1 :

				NrfDataBuild(TxPayloadW,FirstSendingData);
				break;

			case IsFifoEmpty1 :

				ReadFifo();
				break;

			case ClearFifo1 :

				ClearFifo(FlushTx);
				break;

			case IsConfigReset1:
				ReadChannel();
				break;

			case TransmitCase1 :

				NrfTransmit();
				break;

			case IsReceiveACK1 :

				ReadStatus();
				break;

			case DataBuild2 :

				NrfDataBuild(TxPayloadW,SecondSendingData);
				break;

			case IsFifoEmpty2 :

				ReadFifo();
				break;

			case ClearFifo2 :

				ClearFifo(FlushTx);
				break;

			case IsConfigReset2:

				ReadChannel();
				break;

			case TransmitCase2 :

				NrfTransmit();
				break;

			case IsReceiveACK2 :
				ReadStatus();
				holdingtimeNrf=xTaskGetTickCount();
				break;
		}
	}

}

void NrfDataBuild(uint8_t TxType , uint8_t Selection)
{
	// "ST:21.56-P:12300-RT:23.45-DL:32-"
	// "GX:2-GY:3-GZ:5-DL:20"

	char buffer[20];
	memset(mNrfCom.Transmit,0,sizeof(mNrfCom.Transmit));
	mNrfCom.Transmit[0]=TxType;
	//HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);

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

			mNrfCom.Transmit[1]=Selection;
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
			strcat(mNrfCom.Transmit,"-T:");

			memset(buffer,0,sizeof(buffer));
			sprintf(buffer,"%d",PeriodicNrfDataTransmitionTime);
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
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCase++;
	HAL_SPI_Transmit_DMA(mSpi, (uint8_t *) mNrfCom.Transmit, 33);
}

void ClearFifo(uint8_t Type)
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCase++;
	mNrfCom.DummyTransmit=Type;
	HAL_SPI_Transmit_DMA(mSpi, (uint8_t *)&mNrfCom.DummyTransmit, sizeof(mNrfCom.DummyTransmit));
}

void ReadFifo()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCase++;
	mNrfCom.DummyTransmit=FifoStatus;
	HAL_SPI_Transmit_DMA(mSpi, (uint8_t *)&mNrfCom.DummyTransmit, sizeof(mNrfCom.DummyTransmit));
}

void ReadStatus()
{
	//Delay for transmitting data

	HAL_GPIO_WritePin(NrfChipEnPort, NrfChipEnablePinNum, GPIO_PIN_SET);
	vTaskDelay(pdMS_TO_TICKS(4));
	HAL_GPIO_WritePin(NrfChipEnPort, NrfChipEnablePinNum, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCase++;
	mNrfCom.DummyTransmit=Status;
	HAL_SPI_Transmit_DMA(mSpi, (uint8_t *)&mNrfCom.DummyTransmit, sizeof(mNrfCom.DummyTransmit));
}

void ReadChannel()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCase++;
	mNrfCom.DummyTransmit=RfChanel;
	HAL_SPI_Transmit_DMA(mSpi, (uint8_t *)&mNrfCom.DummyTransmit, sizeof(mNrfCom.DummyTransmit));
}

void NRFDelayMicroSeconds(uint32_t uSec)
{
	uint32_t uSecVar = uSec;
	uSecVar = uSecVar* ((SystemCoreClock/1000000)/3);
	while(uSecVar--);
}
