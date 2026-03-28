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

#if UsingControlCenter
#include <string>
#include <SensorsData.h>
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
NrfCase mNrfCase=IsConfigReset;

uint32_t holdingtimeNrf=0;
int NrfTimeOut=0;
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

void NrfDataReceiving()
{
	if(holdingtimeNrf>PeriodicNrfDataReceiveTime )//&& NrfReceivingDataFlag==true)
	{
		switch(mNrfCase)
		{
			case IsConfigReset :

				ReadChannel();
				break;

			case ReadStatusFifo :

				ReadFifo();
				break;

			case NrfReceive :

				ReadingRxFifo();
				break;

			case NrfParse :
				NrfParseData();
				break;
		}

		holdingtimeNrf=0;
	}

}

void ReadingRxFifo()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCom.DummyTransmit=RxPayloadR;
	mNrfCase=NrfReceiveTR;
	HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&mNrfCom.DummyTransmit, (uint8_t *) mNrfCom.Receive, sizeof(mNrfCom.Receive));
}


void NrfGetData(uint8_t *i)
{
	static bool FirstDataFlag=false;
	static bool LastDataFlag=false;
	static uint8_t FirsDataIndexHolder=0;
	std::string DataLength;

	if(mNrfCom.Receive[*i]=='D')
	{
		FirstDataFlag=true;
		FirsDataIndexHolder=*i;
	}
	else if(FirstDataFlag==true && mNrfCom.Receive[*i]=='L')
	{
		LastDataFlag=true;
	}
	else if(LastDataFlag==true && mNrfCom.Receive[*i] =='-')
	{
		//memcpy(DataLength,(mNrfCom.Receive + FirsDataIndexHolder + 3), i - (mNrfCom.Receive + FirsDataIndexHolder + 3 -1))
		DataLength=mNrfCom.Receive;

		DataLength=DataLength.substr(FirsDataIndexHolder + 3, (*i - ( FirsDataIndexHolder + 3 -1)));
		mNrfCom.ReceiveDataLength=stoi(DataLength);

	//		NrfReceivingDataFlag=false; // go to other receiving data block
			holdingtimeNrf=0;
		//	mNrfCase=SendReadingCommand;
			i=0;
	//		HAL_GPIO_WritePin(GPIOC, NrfChipSelect_Pin, GPIO_PIN_SET);

	}

	if(*i>=31)
	{
	//	NrfReceivingDataFlag=false; // go to other receiving data block
		holdingtimeNrf=0;
	//	mNrfCase=SendReadingCommand;
		i=0;
//		HAL_GPIO_WritePin(GPIOC, NrfChipSelect_Pin, GPIO_PIN_SET);
	}
//	mNrfCase=ReadingFifo;

}

void NrfParseData()
{
	std::string NrfPtr(mNrfCom.Receive,strlen(mNrfCom.Receive));
	int Pos1=0;
	int Pos2=0;
	int DataLength = NrfPtr.length();

	if(DataLength>33) DataLength=33;

	while(Pos2 < DataLength - 1)
	{
		Pos1=NrfPtr.find(':', Pos1 + 1);
		Pos2=NrfPtr.find('-', Pos2 + 1);


		if(Pos1<0 || Pos2<0)
		{

			SensorsData.Get.ParseFlag=false;  // Transmit Nrf sended wrong type
			break;
		}
		else if(*(mNrfCom.Receive+1)== SecondSendingData)
		{
			if(NrfPtr.substr(Pos1-2, 2)=="GX")
			{
				SensorsData.Get.GX= std::stof(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
			}
			else if(NrfPtr.substr(Pos1-2, 2)=="GY")
			{
				SensorsData.Get.GY= std::stof(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
			}
			else if(NrfPtr.substr(Pos1-2, 2)=="GZ")
			{
				SensorsData.Get.GZ= std::stof(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
			}
			else if(NrfPtr.substr(Pos1-1, 1)=="T")
			{
				SensorsData.Get.NrfTimeout= std::stof(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
			}
		}
		else
		{
			if(NrfPtr.substr(Pos1-2, 2)=="ST")
			{
				SensorsData.Get.SecTemp= std::stof(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
			}
			else if(NrfPtr.substr(Pos1-1, 1)=="P")
			{
				SensorsData.Get.Press= std::stoi(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
				SensorsData.PressToHeight(SensorsData.Get.Press);
			}
			else if(NrfPtr.substr(Pos1-2, 2)=="RT")
			{
				SensorsData.Get.RoomTemp= std::stof(NrfPtr.substr(Pos1 + 1, Pos2-Pos1-1));
			}
		}

		SensorsData.Get.ParseFlag=true;
	}

	mNrfCase=IsConfigReset;

}

void ReadFifo()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCom.DummyTransmit=FifoStatus;
	mNrfCase=ReadStatusFifoTR;
	HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&mNrfCom.DummyTransmit,(uint8_t *)mNrfCom.DummyReceive, 2);
}

void ReadChannel()
{
	HAL_GPIO_WritePin(NrfChipSelectPort, NrfChipSelectPinNum, GPIO_PIN_RESET);
	mNrfCase=IsConfigResetTR;
	mNrfCom.DummyTransmit=RfChanel;
	HAL_SPI_TransmitReceive_DMA(&hspi2, (uint8_t *)&mNrfCom.DummyTransmit,(uint8_t *)mNrfCom.DummyReceive, 2);
}

void NRFDelayMicroSeconds(uint32_t uSec)
{
	uint32_t uSecVar = uSec;
	uSecVar = uSecVar* ((SystemCoreClock/1000000)/3);
	while(uSecVar--);
}
