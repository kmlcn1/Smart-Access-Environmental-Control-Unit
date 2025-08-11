/*
 * Tmc2208.cpp
 *
 *  Created on: Aug 9, 2025
 *      Author: Kemal
 */

#include <Tmc2208.h>
#include <Mpu6050.h>
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "tim.h"
#include <math.h>

Step mStep;
StepMotor mStepMotor={.isReady=true,.SetSpeed=50,.step=0};
uint8_t TypeOfParameter;
bool isReady=true;
uint32_t counter;
uint16_t step;
uint16_t TIM_ARR=0;

void Tmc2208Init()
{
	SetStepMode(Step16);

}

void SetStepMode(uint8_t parameter)
{

	mStepMotor.TypeOfParameter=parameter;

	switch(parameter)
	{

		case HalfStep :

			HAL_GPIO_WritePin(GPIOB, StepMotorMS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, StepMotorMS2_Pin, GPIO_PIN_RESET);
			break;

		case QuarterStep :
			HAL_GPIO_WritePin(GPIOB, StepMotorMS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, StepMotorMS2_Pin, GPIO_PIN_SET);
			break;

		case Step8 :

			HAL_GPIO_WritePin(GPIOB, StepMotorMS1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOB, StepMotorMS2_Pin, GPIO_PIN_RESET);
			break;

		case Step16 :

			HAL_GPIO_WritePin(GPIOB, StepMotorMS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, StepMotorMS2_Pin, GPIO_PIN_SET);
			break;
	}
}


void SetMotorDirection(MotorDirection direction)
{
    if (direction == MotorDirectionCW)
    {
        HAL_GPIO_WritePin(GPIOB, StepMotorDir_Pin, GPIO_PIN_SET);
   //     vTaskDelay(pdMS_TO_TICKS(500));
    } else if (direction == MotorDirectionCCCW)
    {
        HAL_GPIO_WritePin(GPIOB, StepMotorDir_Pin, GPIO_PIN_RESET);
   //     vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void SetRotatingMovement(float angle)
{

	if(angle!=0 && mStepMotor.isReady==true)
	{
		mStepMotor.isReady=false;

		if(angle<0)
		{
			SetMotorDirection(MotorDirectionCW);
		}
		else
		{
			SetMotorDirection(MotorDirectionCCCW);
		}

		angle=fabs(angle); // absolute value

		mStepMotor.step= angle/1.8*mStepMotor.TypeOfParameter; // Calculate Number of Step

		__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 1); // Set Duty Cycle
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);

	}

}
void SetRotatingSpeed(TIM_HandleTypeDef *htim, uint8_t percentage)
{
	// pres: 100 , ARR:200 , setSpeed: 90(min) = 3870 Hz --> ARR:222
	// pres: 100 , ARR:200 , setSpeed: 200(max) = 8600 Hz -->ARR:100

	if(percentage>100) percentage=100;
	if (percentage<=0) percentage=1;

	htim->Instance->ARR=222 - ((percentage - 1) * 122) / 99;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim3)
	{
		static uint16_t PulseCount=0;
		PulseCount++;
		if(PulseCount==mStepMotor.step)
		{
			HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
			__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
			PulseCount=0;
			mStepMotor.isReady=true;
		}
	}
}
