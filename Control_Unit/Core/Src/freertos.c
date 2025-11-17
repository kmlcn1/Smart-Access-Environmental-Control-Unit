/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <Bme280.h>
#include "i2c.h"
#include <Mpu6050.h>
#include <Tmc2208.h>
#include "tim.h"
#include <Bmp180.h>
#include <Sht31.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId TempHandle;
osThreadId MainHandle;
osThreadId NRFComHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void TempFunc(void const * argument);
void MainFunc(void const * argument);
void NRFComFunc(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityLow, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Temp */
  osThreadDef(Temp, TempFunc, osPriorityHigh, 0, 1280);
  TempHandle = osThreadCreate(osThread(Temp), NULL);

  /* definition and creation of Main */
  osThreadDef(Main, MainFunc, osPriorityNormal, 0, 1280);
  MainHandle = osThreadCreate(osThread(Main), NULL);

  /* definition and creation of NRFCom */
  osThreadDef(NRFCom, NRFComFunc, osPriorityRealtime, 0, 1280);
  NRFComHandle = osThreadCreate(osThread(NRFCom), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_TempFunc */
/**
* @brief Function implementing the Temp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_TempFunc */
void TempFunc(void const * argument)
{
  /* USER CODE BEGIN TempFunc */
  /* Infinite loop */
  for(;;)
  {
//	  if(pos==1 && xTaskGetTickCount()- holdingtimeBme280>= pdMS_TO_TICKS(PeriodicTemperatureTimeforBme280))
//	 {
////		 HAL_GPIO_WritePin(GPIOC,Pin_Select_1_Pin,GPIO_PIN_RESET);
//			Bme280_Raw_Temp(Room1Spi_Cs_Pin,Calib.temp1);
//			Bme280_Raw_Temp(Room2Spi_Cs_Pin,Calib.temp2);
//			Bme280_Raw_Temp(Room3Spi_Cs_Pin,Calib.temp3);
//			Bme280_Raw_Temp(Room4Spi_Cs_Pin,Calib.temp4);
//			holdingtimeBme280=xTaskGetTickCount();
//	 }

	  Sht31_Temperature();
	  ReadBmp180PressureData();
	  osDelay(1);
  }
  /* USER CODE END TempFunc */
}

/* USER CODE BEGIN Header_MainFunc */
/**
* @brief Function implementing the Main thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MainFunc */
void MainFunc(void const * argument)
{
  /* USER CODE BEGIN MainFunc */
  /* Infinite loop */
  for(;;)
  {
	Mpu6050Read();
	ActiveStabilizationPlatform();
    osDelay(1);
  }
  /* USER CODE END MainFunc */
}

/* USER CODE BEGIN Header_NRFComFunc */
/**
* @brief Function implementing the NRFCom thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_NRFComFunc */
void NRFComFunc(void const * argument)
{
  /* USER CODE BEGIN NRFComFunc */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END NRFComFunc */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
