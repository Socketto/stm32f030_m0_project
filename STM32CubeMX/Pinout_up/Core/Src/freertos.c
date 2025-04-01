/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "PJLeds.h"
#include "cmsis_os.h"
#include "usart.h"
#include "adc.h"
#include "VTSP_display.h"
#include <stdio.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     

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
osThreadId pjTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  //osThreadDef(DisplayTask, Refresh_PJ, osPriorityNormal, 1, 128);
  osThreadDef(DisplayTask, Refresh_LCD, osPriorityNormal, 1, 128);
  pjTaskHandle =  osThreadCreate(osThread(DisplayTask), NULL);

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
uint32_t valueTemp[20],counterADC=200,i;
float avg = 0;
uint8_t valuestring[] = "---";
uint8_t buffer[1];

void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
	  osDelay(1);
	  counterADC--;
	  if(counterADC == 0)
	  {
		  counterADC = 200;

			  avg = 0;
			  for(i=0;i<20;i++)
			  {

				  HAL_ADC_Start(&hadc);
				  if (HAL_ADC_PollForConversion(&hadc, 100000000) == HAL_OK)
				  {
					  valueTemp[i] = HAL_ADC_GetValue(&hadc);
				  }
				  HAL_ADC_Stop(&hadc);
			  }
			  for(i=0;i<20;i++)
			  {
				  avg=avg+valueTemp[i];
			  }
			  avg = avg / 20;
			  if(avg<999)
			  {
				  sprintf((char*)valuestring,"%ld",(int32_t)avg);
				  PJ_Display((char*)valuestring);
				  if(avg>300)
				  {
					  PJ_LED(LED_ALARM,1);
				  }
				  else
				  {
					  PJ_LED(LED_ALARM,0);
				  }
			  }
			  else
			 {
				  PJ_Display("Ready for STone");
			  }

	  }
	  else
	  {


		  if(HAL_GPIO_ReadPin(INPUT_1_GPIO_Port,INPUT_1_Pin)==0)
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,1);
			  PJ_Display("ON1");
			  PJ_LED(LED_COMPRESSOR,1);
		  }
		  else
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,0);
			  PJ_LED(LED_COMPRESSOR,0);
		  }

		  if(HAL_GPIO_ReadPin(INPUT_2_GPIO_Port,INPUT_2_Pin)==0)
		  {
			  HAL_GPIO_WritePin(RELE_1_GPIO_Port,RELE_1_Pin,1);
			  PJ_Display("ON2");
			  PJ_LED(LED_FAN,1);
		  }
		  else
		  {
			  HAL_GPIO_WritePin(RELE_1_GPIO_Port,RELE_1_Pin,0);
			  PJ_LED(LED_FAN,0);
		  }

		  if(HAL_GPIO_ReadPin(INPUT_3_GPIO_Port,INPUT_3_Pin)==0)
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,1);
			  PJ_Display("ON3");
			  PJ_LED(LED_COMPRESSOR,1);
		  }
		  else
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,0);
			  PJ_LED(LED_COMPRESSOR,0);
		  }


		  if(HAL_GPIO_ReadPin(INPUT_4_GPIO_Port,INPUT_4_Pin)==0)
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,1);
			  PJ_Display("ON4");
			  PJ_LED(LED_COMPRESSOR,1);
		  }
		  else
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,0);
			  PJ_LED(LED_COMPRESSOR,0);
		  }


		  if(HAL_GPIO_ReadPin(INPUT_5_GPIO_Port,INPUT_5_Pin)==0)
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,1);
			  PJ_Display("ON5");
			  PJ_LED(LED_COMPRESSOR,1);
		  }
		  else
		  {
			  HAL_GPIO_WritePin(RELE_2_GPIO_Port,RELE_2_Pin,0);
			  PJ_LED(LED_COMPRESSOR,0);
		  }
	  }


	  /*try BLUETOOTH <-> RS-232 Communication*/
	  if(HAL_UART_Receive(&huart1, buffer, sizeof(buffer), 0) != HAL_ERROR)
	  {
		  HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);
	  }

	  if(HAL_UART_Receive(&huart2, buffer, sizeof(buffer), 0) != HAL_ERROR)
	  {
		  HAL_GPIO_WritePin(ENABLE_RS485_GPIO_Port,ENABLE_RS485_Pin,1);
		  HAL_UART_Transmit(&huart1, buffer, sizeof(buffer), HAL_MAX_DELAY);
		  HAL_GPIO_WritePin(ENABLE_RS485_GPIO_Port,ENABLE_RS485_Pin,0);
	  }


  }
  /* USER CODE END StartDefaultTask */
}


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
