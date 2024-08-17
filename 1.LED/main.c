/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define DELAY 50
#define DELAY_D1 500
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
  /* USER CODE BEGIN 2 */
 GPIO_InitTypeDef D01_Config;
 D01_Config.Pin = GPIO_PIN_7;
  D01_Config.Mode = GPIO_MODE_OUTPUT_PP;
  D01_Config.Pull = GPIO_NOPULL;
  D01_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  GPIO_InitTypeDef Joy_ON_Config;
 Joy_ON_Config.Pin = GPIO_PIN_15;
  Joy_ON_Config.Mode = GPIO_MODE_INPUT;
  Joy_ON_Config.Pull = GPIO_NOPULL;
  Joy_ON_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  GPIO_InitTypeDef Joy_LEFT_Config;
  Joy_LEFT_Config.Pin = GPIO_PIN_1;
  Joy_LEFT_Config.Mode = GPIO_MODE_INPUT;
  Joy_LEFT_Config.Pull = GPIO_NOPULL;
  Joy_LEFT_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  GPIO_InitTypeDef Joy_RIGHT_Config;
  Joy_RIGHT_Config.Pin = GPIO_PIN_0;
  Joy_RIGHT_Config.Mode = GPIO_MODE_INPUT;
  Joy_RIGHT_Config.Pull = GPIO_NOPULL;
  Joy_RIGHT_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  GPIO_InitTypeDef Joy_DOWN_Config;
  Joy_DOWN_Config.Pin = GPIO_PIN_2;
  Joy_DOWN_Config.Mode = GPIO_MODE_INPUT;
  Joy_DOWN_Config.Pull = GPIO_NOPULL;
  Joy_DOWN_Config.Speed = GPIO_SPEED_FREQ_LOW ;


  GPIO_InitTypeDef Green_Config;
  Green_Config.Pin = GPIO_PIN_8;
  Green_Config.Mode = GPIO_MODE_OUTPUT_PP;
  Green_Config.Pull = GPIO_NOPULL;
  Green_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  GPIO_InitTypeDef Red_Config;
 Red_Config.Pin = GPIO_PIN_13;
  Red_Config.Mode = GPIO_MODE_OUTPUT_PP;
  Red_Config.Pull = GPIO_NOPULL;
  Red_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  GPIO_InitTypeDef Blue_Config;
  Blue_Config.Pin = GPIO_PIN_12;
  Blue_Config.Mode = GPIO_MODE_OUTPUT_PP;
  Blue_Config.Pull = GPIO_NOPULL;
  Blue_Config.Speed = GPIO_SPEED_FREQ_LOW ;

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  HAL_GPIO_Init(GPIOC, &D01_Config);
  HAL_GPIO_Init(GPIOE, &Joy_ON_Config);
  HAL_GPIO_Init(GPIOE, &Joy_LEFT_Config);
  HAL_GPIO_Init(GPIOE, &Joy_RIGHT_Config);
  HAL_GPIO_Init(GPIOE, &Joy_DOWN_Config);
  HAL_GPIO_Init(GPIOD, &Blue_Config);
  HAL_GPIO_Init(GPIOB, &Green_Config);
  HAL_GPIO_Init(GPIOD, &Red_Config);



 // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */




	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	  HAL_Delay(DELAY_D1);














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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

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
