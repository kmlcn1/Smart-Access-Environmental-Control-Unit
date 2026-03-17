/*
 * Lcd.h
 *
 *  Created on: Mar 10, 2026
 *      Author: Kemal
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "stdint.h"
#include "i2c.h"
#include "main.h"
#include <string>


class Lcd
{
	public:

		enum ConstLcd{

			SlaveAdress=			0x27 << 1,
			LcdInitData=			0xD,
			LcdInitInstruct=		0xC,
			LcdDisable=      		0xFB,
			Lcd8BitMode= 			0x30,
			Lcd4BitMode=			0x20,
			Lcd4Bit2Line=			0x28,
			LcdDispOnCurserOff=		0x0C,
			LcdClearDisp=			0x01,
			LcdEntrySet=			0x06,
			HighCase=0,
			HighCaseT=1,
			EnPulseTHigh=2,
			LowCase=3,
			lowCaseT=4,
			EnPulseTLow=5,
			ComplateTransmit=6,
			C=4,

		};


		struct I2CLcd{

			int Transmit[4];
			int Receive[4];
			uint8_t buffer[80];
			uint8_t Data;
			volatile uint8_t TransmitCounter;
			volatile int CaseNum;

		};

		int HoldingTime;
		ConstLcd mConstLcd;
		I2CLcd mI2CLcd;

		void Init();
		void EnPulseInit(uint8_t data , Lcd :: ConstLcd endis);
		void EnPulse(uint8_t data , Lcd :: ConstLcd endis);
		void Transmit(uint8_t *LcdTransmitData);
		void Print(const char *data1 ,const char *data2,
				const char *data3, const char *data4);
		void Example();


	private:


};

extern Lcd mLcd;

#endif /* INC_LCD_H_ */
