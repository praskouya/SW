/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include <stdbool.h>
#include <string.h>
#include "main.h"

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
typedef volatile uint32_t int_32;
typedef struct {
    int_32 MODR;
    int_32 OTYPER;
    int_32 OSPEEDR;
    int_32 PUPDR;
    int_32 IDR;
    int_32 ODR;
    int_32 BSRR;
    int_32 LCKR;
    int_32 AFRL;
    int_32 AFRH;
    int_32 BRR;
    int_32 ASCR;
}*myGPIO; // gpio

typedef struct {
    int_32 CR;
    int_32 ICSCR;
    int_32 CFGR;
    int_32 PLLCFGR;
    int_32 PLLSAI1CFGR;
    int_32 PLLSAI2CFGR;
    int_32 CIER;
    int_32 CIFR;
    int_32 CICR;

    int_32 OCCUPIED_0;

    int_32 AHB1RSTR;
    int_32 AHB2RSTR;
    int_32 AHB3RSTR;

    int_32 OCCUPIED_1;

    int_32 APB1RSTR1;
    int_32 APB1RSTR2;
    int_32 APB2R;

    int_32 OCCUPIED_2;

    int_32 AHB1ENR;
    int_32 AHB2ENR;
    int_32 AHB3ENR;

    int_32 OCCUPIED_3;

    int_32 APB1ENR1;
    int_32 APB1ENR2;
    int_32 APB2ENR;
}*my_RCC; // sterowanie zegarem

typedef struct{
    int_32 CR1;
    int_32 CR2;
    int_32 CR3;
    int_32 BRR;
    int_32 RESERVED1[2];
    int_32 RQR;
    int_32 ISR;
    int_32 ICR;
    int_32 RDR;
    int_32 TDR;
}*myLUPART; // wymiana danych

#define my_LUPART ((myLUPART) 0x40008000)
#define baud_rate ((256 * 4000000)/9600) //9600 - szybkosc trasmisji
typedef char dataSize;

#define myGPIOC ((myGPIO) 0x48000800)
#define myRCC ((my_RCC) 0x40021000)



void EnableCLKC(void){
    myRCC->AHB2ENR |= (1u << 2); // włączenie zegara dla portu gpioc

}

void myGPIOC_init(void){
    myGPIOC->MODR |= ((1u << 3) | (1u <<1));
    myGPIOC->MODR &= ~((1u << 2) | (1u << 0));
    myGPIOC->AFRL |= ((1u << 3) | (1u << 7)); // ustawienie alternatywnej funkcji pinu - na komunikację uart

}

void UART(void){
    myRCC->APB1ENR2 |= (1u << 0); // włączenie zagara dla UART 1
    my_LUPART->CR1 &= ~(1u << 0); // wyłączenie UART
    my_LUPART->BRR = baud_rate; // szybkość transmisji
    my_LUPART->CR1 |= ((1u << 2) | (1u << 3)) ; // włączenie odbiornka i nadajnika
    my_LUPART->CR1 |= ((1u << 0)) ; // włączenie UART
}

#define OFFSET ('a' - 'A')
bool is_lowercase(char letter){
    return letter >= 'a' && letter <= 'z';
}

bool is_uppercase(char letter){
    return letter >= 'A' && letter <= 'Z';
}

char to_uppercase(char letter){
    return letter - OFFSET;
}

char to_lowercase(char letter){
    return letter + OFFSET;
}


void sendData(dataSize data){
    while((my_LUPART->ISR &(1u << 7)) == 0){} // oczekiwanie aż rejestr transmisji danych będzie pusty, bit 7 równy 1 oznacza że tdr jest pusty
    my_LUPART->TDR = data; // zapisanie danych do rejestru, co powoduje ich przeslanie
}

char readData(void){
    char data;
    while((my_LUPART->ISR & (1U << 5)) == 0){} // oczekiwanie aż rejestr RDR nie bedzie pusty, analogicznie do funkcji wyżej
    data = my_LUPART->RDR; // do zmiennej przypisujemy wartość odebranych danych
    return data;
}

#define BUFFER_SIZE 5
char buffer[BUFFER_SIZE];
int buffer_index = 0;

void SendMessage(char msg[]) {
	for (int i = 0; i < (strlen(msg)); i++) {
		sendData(msg[i]);
	}
}

void bufferData(char data) {
    char full[] = "Buffer is full!";
    if (buffer_index < BUFFER_SIZE ) {
        buffer[buffer_index++] = data;
    }
    else {
    	SendMessage(full);
    }
}

void transmitBuffer(void) {
    char empty[] = "Buffer is empty!";
	if (buffer_index > 0) {
	        sendData(buffer[0]);
	        for (int i = 1; i < buffer_index; i++) {
	            buffer[i - 1] = buffer[i];
	        }
	        buffer_index--;
	    }
	else {
    	SendMessage(empty);
	}
}

void transmitWholeBuf(void) {
	for (int i = 0; i < buffer_index; i++) {
	        sendData(buffer[i]);
	    }

	    buffer_index = 0;
}


void emptyBuffer(void) {
    buffer_index = 0;
    char empty[] = "Buffer is flushed!";
	SendMessage(empty);
}

void lupart_transmit(void){ // received data
	 char data;
	    data = readData();
	    if (data == ' ') {
	        transmitBuffer();
	    }
	    else if (data == ',') {
	    	emptyBuffer();
	    }
	    else if (data == '.' ) {
	    	transmitWholeBuf();
	    }
	    else {
	        bufferData(data);
	    }

}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    EnableCLKC();
    myGPIOC_init();
    UART();


    while (1)
    {
        lupart_transmit();
    }

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


