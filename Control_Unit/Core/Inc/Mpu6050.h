/*
 * Mpu6050.h
 *
 *  Created on: Jun 11, 2025
 *      Author: Kemal
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_
#include <stdint.h>


#define Mpu650Address		0x68 //When AD0 pin is low // 01101000 (included W (0) bit)

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
	uint16_t GyroY;
	uint16_t GyroZ;

}Mpu650;


typedef enum _Mpu650Value
{
	GyroAddress=0x1B,
	AccellAddress=0x1C,
	GyroData=0x08,
	AccellData= 0x08,
	AccellXoutMsb=0x3B,
	AccellXoutLsb=0x3C,
	AccellYoutMsb=0x3D,
	AccellYoutLsb=0x3E,
	AccellZoutMsb=0x3F,
	AccellZoutLsb=0x40,
	GyroXoutMsb=0x43,
	GyroXoutLsb=0x44,
	GyroYoutMsb=0x45,
	GyroYoutLsb=0x46,
	GyroZoutMsb=0x47,
	GyroZoutLsb=0x48,
}Mpu650Value;




extern Mpu650Value mMpu650Value;
extern Mpu650Com mMpu650Com;
extern Mpu650 mMpu6050;


void ReadMpu6050AccellandGyro(Mpu650Value adress,char *type);
void ReadMpu6050Gyro(Mpu650Value adress,char *type);
void Mpu6050Read(void);
#endif /* INC_MPU6050_H_ */
