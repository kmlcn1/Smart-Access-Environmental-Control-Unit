/*
 * Nrf2401Plus.h
 *
 *  Created on: Dec 1, 2025
 *      Author: Kemal
 */

#ifndef NRF2401PLUS_H_
#define NRF2401PLUS_H_
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define 		RX						0x20
#define			TX						0x19
#define			RxPipeV					0xAABBCCDDEE
#define			TxPipeV					0xAABBCCDDEE
#define			ChNum					100
#define 		UsingControlUnit    	 1
#define			UsingControlCenter	 	 0

typedef enum _NrfConst{

	WriteMode=					0x20,
	ReadMode=					0x00,
	EnRxPipe1=					0x01,
	AdressWidth5=				0x03, // 5 Byte
	DisAllAA=					0x00,
	EnAAP0=						0x01,
	TxMode=						0x7E,
	RxMode=						0x7F,
	DataRate250kbps= 			0x20,
	DataRate1Mbps=				0x00,
	RFOutputPower0dBm=			0x06,
	RFOutputPowerMinus6dBm=		0x04,
	RFOutputPowerMinus12dBm=	0x02,
	DisAutoReTransmit=			0x00,
	EnableNoAck=				0x01,
	uS250=						0x00,
	uS500=						0x01,
	ReTransmitCount8=			0x08,
	ReTransmitCount10=			0xA,
	ClearAct=					0x20,
	ClearReTransmit= 			0x10,
}NrfConst;

typedef enum _NrfAdress{

	Config=				0x00,
	AutoAck=			0x01, // Enable - Disable auto acknowledgement on data pipes
	EnRxPipe=  			0x02,
	SetupAW=			0x03,
	SetupReTR=			0x04,
	RfChanel=			0x05,
	RfSetup=			0x06,
	Status=				0x07,
	RPD=				0x09,
	RxPipe0Adress=		0x0A,
	TxPipe0Adress=		0x10,
	RxPayload=			0x11,
	FifoStatus=			0x17,
	TxPayloadNoActW=	0xB0,
	TxPayloadW=			0xA0,
	RxPayloadR=			0x61,
	FlushTx=			0xE1,
	FlushRx=			0xE2,
	Feature=			0x1D,
}NrfAdress;


typedef enum _NrfCase {
	DataBuild1=					0x01,
	IsFifoEmpty1=				0x02,
	IsFifoEmpty1T=				0x03,
	IsFifoEmpty1R=				0x04,
	ClearFifo1=					0x05,
	ClearFifo1T=				0x06,
	IsConfigReset1=				0x07,
	IsConfigReset1T=			0x08,
	IsConfigReset1R=			0x09,
	TransmitCase1=				0xA,
	TransmitCase1T=				0xB,
	IsReceiveACK1=				0xC,
	IsReceiveACK1T=				0xD,
	IsReceiveACK1R=				0xE,
	DataBuild2=					0xF,
	IsFifoEmpty2=				0x10,
	IsFifoEmpty2T=				0x11,
	IsFifoEmpty2R=				0x12,
	ClearFifo2=					0x13,
	ClearFifo2T=				0x14,
	IsConfigReset2=				0x15,
	IsConfigReset2T=			0x16,
	IsConfigReset2R=			0x17,
	TransmitCase2=				0x18,
	TransmitCase2T=				0x19,
	IsReceiveACK2=				0x1A,
	IsReceiveACK2T=				0x1B,
	IsReceiveACK2R=				0x1C,
	FirstSendingData=			0x1D,
	SecondSendingData=			0x1E,
	IsConfigReset=				0x1F,
	IsConfigResetTR=			0x20,
	ReadStatusFifo=				0x21,
	ReadStatusFifoTR=			0x22,
	NrfReceive=					0x23,
	NrfReceiveTR=				0x24,

}NrfCase;



typedef struct _NrfCom{

	char Receive[32];
	char DummyReceive[2];
	char DummyTransmit;
	char Transmit[33];
	uint8_t ReceiveDataLength;
	uint8_t TransmitInit[6];

}NrfCom;



extern NrfCase mNrfCase;
extern NrfAdress mNrfAdress;
extern NrfConst mNrfConst;
extern NrfCom mNrfCom;
extern uint32_t holdingtimeNrf;
extern bool NrfReceivingDataFlag;
extern GPIO_TypeDef* NrfChipSelectPort;
extern uint16_t NrfChipSelectPinNum;
extern GPIO_TypeDef* NrfChipEnPort;
extern uint16_t NrfChipEnablePinNum;
extern SPI_HandleTypeDef *mSpi;

void NrfInit(uint8_t Select, GPIO_TypeDef* ChipEnPort, uint16_t ChipEnPin, GPIO_TypeDef* ChipSelectPort, uint16_t ChipSelectPin, SPI_HandleTypeDef *SpiType );
void NrfMode(uint8_t ModeSelect);
void NrfEnableRxPipe(uint8_t Enable);
void NrfPipe(uint8_t Adress1 , uint64_t Activate);
void NrfAutoAck(uint8_t Data);
void NrfSetupAdressWidth(uint8_t Width);
void NrfFreqChannel(uint8_t Channel);
void NrfRFSetup(uint8_t DataRate , uint8_t RfOutputPower);
void NrfRxPayload(uint8_t PayloadSize);
void NrfSetupAutoRetransmit(uint8_t Delay, uint8_t ReTransCount);
void NrfSetupFeature(uint8_t Value);
void NrfClearAct();
void NrfClearReTransmitCount();
void NrfDataSending();
void NrfDataBuild(uint8_t TxType , uint8_t Selection);
void NrfTransmit();
void ClearFifo(uint8_t Type);
void ReadFifo();
void ReadStatus();
void ReadChannel();
void NRFDelayMicroSeconds(uint32_t uSec);

#endif /* NRF2401PLUS_H_ */
