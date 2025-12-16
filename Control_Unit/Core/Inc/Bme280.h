/*
 * Bme280.h
 *
 *  Created on: Oct 20, 2024
 *      Author: KMLCN
 */

#ifndef INC_Bme280_H_
#define INC_Bme280_H_

#include <stdint.h>
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
	uint8_t Receive_Temp[6];
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

#endif /* INC_Bme280_H_ */
