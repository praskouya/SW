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
#define DELAY_D1 10
#define DELAY_D2 500
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
void Show_Numbers (uint16_t tab[10] );
void Change_Window (uint16_t okno[4]);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

 int cur = 0;
 int cur_okno = 0;
 int if_pressed_left = 0;
 int if_pressed_right = 0;

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



           __HAL_RCC_GPIOG_CLK_ENABLE();
           __HAL_RCC_GPIOB_CLK_ENABLE();
           __HAL_RCC_GPIOE_CLK_ENABLE();

           HAL_PWREx_EnableVddIO2();





uint16_t tab[] =
    {GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5,
     GPIO_PIN_1 | GPIO_PIN_2,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_3 | GPIO_PIN_4,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_6,
   GPIO_PIN_1 | GPIO_PIN_2  | GPIO_PIN_5 | GPIO_PIN_6,
    GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3  | GPIO_PIN_5 | GPIO_PIN_6,
    GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 ,
     GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6,
    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_6};

GPIO_InitTypeDef GPIO_Configurate;
  GPIO_Configurate.Pin = GPIO_PIN_0 |GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 |GPIO_PIN_9  ;
  GPIO_Configurate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Configurate.Pull = GPIO_NOPULL;
  GPIO_Configurate.Speed = GPIO_SPEED_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_Configurate);
  GPIO_Configurate.Pin = GPIO_PIN_2 |GPIO_PIN_3 |GPIO_PIN_4| GPIO_PIN_5;
    HAL_GPIO_Init(GPIOB, &GPIO_Configurate);
  GPIO_Configurate.Pin = GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_Configurate.Mode = GPIO_MODE_INPUT;
    GPIO_Configurate.Pull = GPIO_NOPULL;
    GPIO_Configurate.Speed = GPIO_SPEED_LOW;
    HAL_GPIO_Init(GPIOE, &GPIO_Configurate);

uint16_t okno[] = {GPIO_PIN_2, GPIO_PIN_3,GPIO_PIN_4,GPIO_PIN_5 };



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	if (cur > 9) {
		cur = 0;
		 		  }
	else if (cur < 0) {
		cur = 9;
		 		  }



	Change_Window(okno);
    Show_Numbers(tab);


  }




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */









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
void Show_Numbers (uint16_t tab[10]) {
	HAL_GPIO_WritePin(GPIOG, tab[cur],GPIO_PIN_SET);


	    HAL_Delay (DELAY_D2 / DELAY_D1);

		HAL_GPIO_WritePin(GPIOG,tab[cur], GPIO_PIN_RESET);

				 	int button_pushed = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_15) == GPIO_PIN_RESET ? 1:0;
				 	if (HAL_GetTick() % DELAY_D1 == 0) {

				 				 				 		      if (button_pushed) {

				 				 				 		    	 cur--;
				 				 				 		      }

				 				 				 		      else {
				 				 				 		    	  cur++;

				 				 				 		    	  }

				 	}

}

void Change_Window (uint16_t okno[4]) {
	 HAL_GPIO_WritePin(GPIOB, okno[cur_okno],GPIO_PIN_SET);

			  		  	  int left_pushed = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_1) == GPIO_PIN_RESET ? 1:0;
			  		  	  if (left_pushed && !if_pressed_left) {

			  		  		  HAL_GPIO_WritePin(GPIOB, okno[cur_okno],GPIO_PIN_RESET);

			  		  		  cur_okno--;
			  		  		  if (cur_okno < 0) {
			  		  		  	  cur_okno = 0;
			  		  		  	  		}
			  		  		  if_pressed_left = 1;

			  		  	    }

			  		  	    if(!left_pushed) {
			  		  	    	if_pressed_left = 0;
			  		  	    }

			  		  	  int right_pushed = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_0) == GPIO_PIN_RESET ? 1:0;
			  		  	  if (right_pushed && !if_pressed_right) {
			  		  		HAL_GPIO_WritePin(GPIOB, okno[cur_okno],GPIO_PIN_RESET);
			  		  	  	cur_okno++;

			  		  	  	if (cur_okno > 3) {
			  		  	  		cur_okno = 3;
			  		  	  	}
			  		  	  	if_pressed_right = 1;
			  		  	  }

			  		  	  if(!right_pushed) {
			  		  	  	    	if_pressed_right = 0;
			  		  	  	    }


			  		  	HAL_GPIO_WritePin(GPIOB, okno[cur_okno],GPIO_PIN_SET);

	  }

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
