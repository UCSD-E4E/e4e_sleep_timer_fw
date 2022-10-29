/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "main.h"
#include "dma.h"
#include "lptim.h"
#include "usart.h"
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <product.h>
#include <E4E_HAL_System.h>
#include <e4e_common.h>
#include <E4E_ST_App.h>
#include <stm32g0xx.h>
#include <parser.h>
#include <callbacks.h>
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

/* USER CODE BEGIN PV */
E4E_ST_AppDesc_t app;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  E4E_ST_AppConfig_t appConfig;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_LPTIM1_Init();

  // MX_DMA_Init enables DMA clock and UART_init enables DMA channels, need to initialize DMA first
  MX_DMA_Init();
  MX_LPUART1_UART_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */

  int64_t currTime = 0;


  if(E4E_ERROR == E4E_HAL_System_init())
  {
	  while(0)
	  {

	  }
  }


  // Initialize parser
  E4E_HAL_PwrCtrl_setState(&pHalSystem->onboardComputerDesc, E4E_HAL_PWRCTRL_State_ON);
  E4E_BinaryPacket_registerCallback(E4E_BinaryPacket_CMD_SET_ALARM, setAlarmCallback);
  E4E_BinaryPacket_registerCallback(E4E_BinaryPacket_CMD_GET_TIME, getTimeCallback);
  E4E_BinaryPacket_registerCallback(E4E_BinaryPacket_CMD_CLEAR_ALARM, clearAlarmCallback);
  E4E_HAL_RTC_registerAlarmCallback(&pHalSystem->rtcDesc, E4E_App_AlarmExecuteCallback, NULL);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if E4E_APPLICATION_LOGIC == MAIN_APP_LOGIC
#if 0
	  if(E4E_ERROR == E4E_ST_App_init(&app, &appConfig))
	  {
		  while(1)
		  {
			  ;
		  }
	  }

	  while(1)
	  {
		  E4E_ST_App_run(&app);
	  }
#endif
	  uint8_t nextInputByte;

	  while(1)
	  {
		  if(E4E_ERROR == E4E_HAL_Serial_read(&pHalSystem->debugSerialDesc, &nextInputByte, 1, UINT32_MAX))
		  {
			  // Reset the parser
			  E4E_BinaryPacket_reset();
			  E4E_HAL_Serial_flush(&pHalSystem->debugSerialDesc);
			  continue;
		  }

		  if(E4E_ERROR == parse(nextInputByte))
		  {
			  // Reset the parser
			  E4E_BinaryPacket_reset();
		  }
	  }
#elif E4E_APPLICATION_LOGIC == SERIAL_DEBUG_LOGIC
	  // debug logic
#elif E4E_APPLICATION_LOGIC == RTC_DEBUG_LOGIC
	  // debug logic
	  E4E_HAL_RTC_getTime(&pHalSystem->rtcDesc, &currTime);
	  HAL_Delay(500);

#elif E4E_APPLICATION_LOGIC == PWR_CTRL_DEBUG_LOGIC
	  // debug logic
#elif E4E_APPLICATION_LOGIC == CMD_DEBUG_LOGIC
	  // debug logic
#elif E4E_APPLICATION_LOGIC == APP_DEBUG_LOGIC
	  // debug logic
#elif E4E_APPLICATION_LOGIC == DEBUG_APP_LOGIC
	  E4E_DebugApp();
#endif
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
