/*
 * Mpu6050.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Kemal
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_
#include <stdint.h>


#define Mpu650Address		0x68<<1 //When AD0 pin is low // 01101000

typedef struct _Mpu650Com
{
	char Receive[2];
	char Transmit[2];

}Mpu650Com;

typedef struct _Mpu650
{
	uint8_t RawAccellX[2];
	uint8_t RawAccellY[2];
	uint8_t RawAccellZ[2];
	uint8_t RawGyroX[2];
	uint8_t RawGyroY[2];
	uint8_t RawGyroZ[2];
	uint16_t AccellX;
	uint16_t AccellY;
	uint16_t AccellZ;
	uint16_t GyroX;
	float GyroXAngleDif;
	uint16_t GyroY;
	float GyroYAngleDif;
	uint16_t GyroZ;

}Mpu650;


typedef enum _Mpu650Value
{
	GyroAddress=0x1B,
	AccellAddress=0x1C,
	Mpu6050ConfigAdress=0x1A,
	SignalPathResetAdress=0x68,
	PowerManagement1Adress=0x6B,
	PowerManagement2Adress=0x6C,
	Mpu6050ConfigData=0x02, 		// (Output rate 1 khz, Accel: 94Hz 3ms delay, Gyro:98Hz, 2.8 Delay)
	SampleRateDividerAdress=0x19,
	SampleRateDividerData=0x00, 	// (sample rate 1khz )
	SignalPathResetData=0x00,		// (All accel and gyro signal path reset )
	GyroConfigData=0x00,			// (all axis self test disable, +/-250 degree/second)
	AccellConfigData=0x10,			// (all axis self test disable, +/- 8g)
	PowerManagement1Data=0x00,		// (Reset disable, Cycle Mod, Sleep Disable, Temp Dis, İnternal 8Mhz Oscillator )
	PowerManagement1DataFull=0xFF,
	PowerManagement2Data=0x00,		// (wake up freq 1.25Hz, gyro and accel all axis not set standby )
	AccellXoutH=0x3B,
	AccellXoutL=0x3C,
	AccellYoutH=0x3D,
	AccellYoutL=0x3E,
	AccellZoutH=0x3F,
	AccellZoutL=0x40,
	GyroXoutH=0x43,
	GyroXoutL=0x44,
	GyroYoutH=0x45,
	GyroYoutL=0x46,
	GyroZoutH=0x47,
	GyroZoutL=0x48,
}Mpu650Value;




extern Mpu650Value mMpu650Value;
extern Mpu650Com mMpu650Com;
extern Mpu650 mMpu6050;

void InitMpu6050(void);
void ReadMpu6050AccellandGyro(Mpu650Value AdressH);
void ReadMpu6050Gyro(Mpu650Value adress,char *type);
void Mpu6050Read(void);
void TransferReceiverDataforMpu6050(char *in , uint8_t *out);
#endif /* INC_MPU6050_H_ */
