/*
 * Tmc2208.h
 *
 *  Created on: Aug 9, 2025
 *      Author: Kemal
 */

#ifndef INC_TMC2208_H_
#define INC_TMC2208_H_

#include <stdint.h>
#include <stdbool.h>
#include "tim.h"

typedef enum _Step{

	FullStep=1,
	HalfStep=2,
	QuarterStep=4,
	Step8=8,
	Step16=16,
}Step;

typedef enum {
    MotorDirectionCW,   // Clockwise
    MotorDirectionCCCW   // CounterClockWise
} MotorDirection;

typedef struct _StepMotor{
	uint8_t TypeOfParameter;
	volatile bool isReady;
	uint16_t step;
	uint8_t SetSpeed;
}StepMotor;



extern StepMotor mStepMotor;
extern  bool isReady;


void SetRotatingSpeed(TIM_HandleTypeDef *htim, uint8_t percentage);
void Tmc2208Init();
void SetStepMode(uint8_t parameter);
void SetMotorDirection(MotorDirection direction);
void SetRotatingMovement(float angle);
#endif /* INC_TMC2208_H_ */
