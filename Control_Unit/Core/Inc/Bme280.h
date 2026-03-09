/*
 * Bme280.h
 *
 *  Created on: Oct 20, 2024
 *      Author: KMLCN
 */

#ifndef INC_Bme280_H_
#define INC_Bme280_H_

#include <stdint.h>

typedef enum _Bme280Const{
	PressOverSampX1=			0x01,
	PressOverSampX2=			0x02,
	PressOverSampX4=			0x03,
	PressOverSampX8=			0x04,
	PressOverSampX16=			0x05,
	TempOverSampX1=				0x01,
	TempOverSampX2=				0x02,
	TempOverSampX4=				0x03,
	TempOverSampX8=				0x04,
	TempOverSampX16=			0x05,
	SleepMode= 					0x0,
	ForceMode=					0x01,
	NormalMode=					0x03,

	Tstanby0Dot5Ms=				0x0,
	Tstanby62Dot5Ms=			0x01,
	Tstanby125Ms=				0x02,
	Tstanby250Ms=				0x03,
	Tstanby500Ms=				0x04,
	Tstanby1000Ms=				0x05,
	Tstanby10Ms=				0x06,
	Tstanby20Ms=				0x07,
	FilterCoeffNon=				0x0,
	FilterCoeff2=				0x01,
	FilterCoeff4=				0x02,
	FilterCoeff8=				0x03,
	FilterCoeff16=				0x04,

	RegCtrMeas= 0x74,
	RegConfig= 0x75,
	RegCtrMeasSettings=		PressOverSampX16 << 5 | TempOverSampX2 << 2 | NormalMode,
	RegConfigSettings=		Tstanby125Ms << 5 | FilterCoeff16 << 2 | 0x00,

	SetRegister=				0x01,
	GetCalibValue=				0x02,
	CreateDigitalValue=			0x05,
	AllTempRegister=   			0x01,
	GetRawTemp=					0x04,
	Bme280CompletionProcess=	0x05,

}Bme280Const;



typedef struct _Raw {

	uint8_t temp_msb;
	uint8_t temp_lsb;
	uint8_t temp_xlsb;
	uint8_t press_msb;
	uint8_t press_lsb;
	uint8_t press_xlsb;
	double adc;

}Raw;

typedef struct _Calibration{

	uint16_t dig_T1;
	int16_t dig_T2;
	int16_t dig_T3;
	uint16_t dig_P1;
	int16_t dig_P2;
	int16_t dig_P3;
	int16_t dig_P4;
	int16_t dig_P5;
	int16_t dig_P6;
	int16_t dig_P7;
	int16_t dig_P8;
	int16_t dig_P9;
}Calibration;

typedef struct _Bme280
{
	uint8_t Transmit[4];
	uint8_t Receive[24];
	uint8_t Receive_Temp[3];
	uint8_t Receive_Press[18];
	double temp1;
	double temp2;
	double temp3;
	double temp4;
	double press1;
	double press2;
	double press3;
	double press4;
	double humid1;
	double humid2;
	double humid3;
	double humid4;


}Bme280;

typedef struct _Const{
	uint8_t temp_msb;
	uint8_t temp_lsb;
	uint8_t temp_xlsb;
	uint8_t press_msb;
	uint8_t press_lsb;
	uint8_t press_xlsb;
	uint8_t hum_msb;
	uint8_t hum_lsb;
}Const;


extern Bme280Const mBme280Const;
extern Const Adress;
extern Bme280 mBme280;
extern Raw Raw_Temp;
extern uint8_t Calib_Bme280[24];
extern uint8_t state;
extern uint8_t pos;
extern Calibration chamber1;
extern Calibration chamber2;
extern Calibration chamber3;
extern Calibration chamber4;

void Bme280_Init(uint16_t num, Calibration *mCalibm);
void Spi_Transmit(uint8_t *data);
void Raw_Temp_Get();
double Bme280_Temp(Calibration *Calib);
void Bme280_Raw_Temp(uint16_t row, double *temp,Calibration *chamber);
void Bme280_Const_Val_Op(Calibration *mcalib);
void SpiTransmitInit(uint8_t *data);
void Bme280SetReg(uint8_t Reg1, uint8_t Val1, uint8_t Reg2, uint8_t Val2);
#endif /* INC_Bme280_H_ */
