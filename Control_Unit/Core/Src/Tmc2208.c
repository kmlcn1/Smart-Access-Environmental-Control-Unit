/*
 * Tmc2208.cpp
 *
 *  Created on: Aug 9, 2025
 *      Author: Kemal
 */

#include <Tmc2208.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "main.h"
#include "tim.h"


Step mStep;
uint8_t TypeOfParameter;
void Tmc2208Init()
{
	SetStepMode(HalfStep);
}

void SetStepMode(uint8_t parameter)
{

	TypeOfParameter=parameter;

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


void SetDirectionOfRotating(char *direction)
{
	if(strcmp("ClockWise",direction)==0)
	{
		HAL_GPIO_WritePin(GPIOB, StepMotorDir_Pin, GPIO_PIN_SET);
	}
	else if(strcmp("CounterClockWise",direction)==0)
	{
		HAL_GPIO_WritePin(GPIOB, StepMotorDir_Pin, GPIO_PIN_RESET);
	}
}

void SetRotatingMovement(uint8_t angle)
{
	uint16_t step= angle/(TypeOfParameter*1.8); // Calculate Number of Step

	__HAL_TIM_SET_COUNTER(&htim3,htim3.Init.Period-step) ; // Set ARR register for up counting
	HAL_TIM_PWM_Start_IT(&htim3, HAL_TIM_ACTIVE_CHANNEL_1);

}


void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim==&htim3)
	{
		HAL_TIM_PWM_Stop(&htim3, HAL_TIM_ACTIVE_CHANNEL_1);
	}
}
