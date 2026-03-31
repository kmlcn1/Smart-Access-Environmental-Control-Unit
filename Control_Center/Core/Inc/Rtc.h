/*
 * Rtc.h
 *
 *  Created on: Mar 30, 2026
 *      Author: Kemal
 */

#ifndef INC_RTC_H_
#define INC_RTC_H_

#include "stdint.h"


class mRtc {

public:

	struct mTime{

		int Year;
		int Month;
		int Date;
		int Day;
		int Hour;
		int Minute;
		int Second;
		int UnixTime;
	};

	struct mCom{

		uint8_t Transmit[2];
		uint8_t TransmitInit[7];
		uint8_t Receive[7];
	};

	enum Const{

		Adress=  				0x68 << 1 , // 7 bit Device Adress
		SecAdress= 				0x00,
		MinAdress=				0x01,
		HourAdress= 			0x02,
		DayAdress= 				0x03,
		DateAdress=				0x04,
		MonthAdress= 			0x05,
		YearAdress=				0x06,




	};

	int HoldingRtcTime=0;
	mCom  Com;
	mTime Time;
	void GetDate();
	void Init();
	void GetFromBcd(uint8_t *Data);
	void SetBcdFormat(uint8_t *Data);
	void UnixToRealTime();

private:
};

extern mRtc Rtc;


#endif /* INC_RTC_H_ */
