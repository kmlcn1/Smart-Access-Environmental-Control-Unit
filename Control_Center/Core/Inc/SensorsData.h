/*
 * SensorsData.h
 *
 *  Created on: Mar 23, 2026
 *      Author: Kemal
 */

#ifndef INC_SENSORSDATA_H_
#define INC_SENSORSDATA_H_

class mSensorsData {

public:

	 struct mGet{

		 bool ParseFlag;
		 bool NrfReceive;
		 float SecTemp;
		 int Press;
		 int NrfTimeout;
		 float Altitude;			//meter
		 float RoomTemp;
		 float GX;
		 float GY;
		 float GZ;
	};

	 struct mConst{
		 const float GasConstant= 8.3144;	// J/(mol·K)
		 const float MolarMass = 0.02896;	// kg/mol
		 const float g = 9.81;				// m/s²
		 const float P0 = 101325;			// Pa
		 const float T0 = 288.15;			// Kelvin (K)
		 const float L = 0.0065;			// K/m

	 };

	mGet Get;
	mConst Const;
	void PressToHeight(int Press);
	void ShowAllData();

private:

};

extern mSensorsData SensorsData;

#endif /* INC_SENSORSDATA_H_ */
