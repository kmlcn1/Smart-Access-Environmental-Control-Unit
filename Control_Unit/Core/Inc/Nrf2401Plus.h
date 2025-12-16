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




typedef enum _NrfConst{

	WriteMode=					0x20,
	ReadMode=					0x00,
	EnRxPipe1=					0x01,
	RxPipeV=					0xAABBCCDDEE,
	TxPipeV=					0xAABBCCDDEE,
	AdressWidth5=				0x03, // 5 Byte
	DisAllAA=					0x00,
	TxMode=						0x02,
	RxMode=						0x03,
	DataRate250kbps= 			0x20,
	DataRate1Mbps=				0x00,
	RFOutputPower0dBm=			0x03,
	RFOutputPowerMinus6dBm=		0x02,
	RFOutputPowerMinus12dBm=	0x01,
	DisAutoReTransmit=			0x00,
}NrfConst;

typedef enum _NrfAdress{

	Config=				0x00,
	AutoAck=			0x01, // Enable - Disable auto acknowledgement on data pipes
	EnRxPipe=  			0002,
	SetupAW=			0x03,
	SetupReTR=			0x04,
	RfChanel=			0x05,
	RfSetup=			0x06,
	RPD=				0x09,
	RxPipe0=			0x0A,
	TxPipe0=			0x10,
	RxPayload=			0x11,
	TxPayloadNoActW=	0xB0,
	TxPayloadW=			0xA0,
	RxPayloadR=			0xA1,

}NrfAdress;


typedef enum _NrfCase {
	DataBuild1=					0x01,
	TransmitCase1=				0x02,
	DataBuild2=					0x04,
	TransmitCase2=				0x05,
	FirstSendingData=			0x08,
	SecondSendingData=			0x09,
	RX=							0x10,
	TX=							0x11,
}NrfCase;



typedef struct _NrfCom{

	char Receive[32];
	char DummyReceive;
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



void NrfInit(uint8_t Select);
void NrfMode(uint8_t ModeSelect);
void NrfEnableRxPipe(uint8_t Enable);
void NrfPipe(uint8_t Adress , uint64_t Activate);
void NrfAutoAck(uint8_t Data);
void NrfSetupAdressWidth(uint8_t Width);
void NrfFreqChannel(uint8_t Channel);
void NrfRFSetup(uint8_t DataRate , uint8_t RfOutputPower);
void NrfRxPayload(uint8_t PayloadSize);
void NrfSetupAutoRetransmit(uint8_t Value);
void NrfDataSending();
void NrfDataBuild(uint8_t TxType , uint8_t Selection);
void NrfTransmit();



#endif /* NRF2401PLUS_H_ */
