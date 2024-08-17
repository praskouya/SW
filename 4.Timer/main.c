#include "main.h"
#include <stdint.h>
#include "functions.h"


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define BIT_MASK(x) (1u << (x))

#define PIN_0 BIT_MASK(0)
#define PIN_1 BIT_MASK(1)
#define PIN_2 BIT_MASK(2)
#define PIN_3 BIT_MASK(3)
#define PIN_4 BIT_MASK(4)
#define PIN_5 BIT_MASK(5)
#define PIN_6 BIT_MASK(6)
#define PIN_7 BIT_MASK(7)
#define PIN_8 BIT_MASK(8)
#define PIN_9 BIT_MASK(9)
#define PIN_10 BIT_MASK(10)
#define PIN_11 BIT_MASK(11)
#define PIN_12 BIT_MASK(12)
#define PIN_13 BIT_MASK(13)
#define PIN_14 BIT_MASK(14)
#define PIN_15 BIT_MASK(15)

#define DIGIT_0 (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4 | PIN_5)
#define DIGIT_1 (PIN_1 | PIN_2)
#define DIGIT_2 (PIN_0 | PIN_1 | PIN_6 | PIN_4 | PIN_3)
#define DIGIT_3 (PIN_0 | PIN_1 | PIN_6 | PIN_2 | PIN_3)
#define DIGIT_4 (PIN_1 | PIN_2 | PIN_6 | PIN_5)
#define DIGIT_5 (PIN_0 | PIN_5 | PIN_6 | PIN_2 | PIN_3)
#define DIGIT_6 (PIN_0 | PIN_2 | PIN_3 | PIN_4 | PIN_5 | PIN_6)
#define DIGIT_7 (PIN_0 | PIN_1 | PIN_2)
#define DIGIT_8 (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4 | PIN_5 | PIN_6)
#define DIGIT_9 (PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_5 | PIN_6)

#define DISP_1 PIN_2
#define DISP_2 PIN_3
#define DISP_3 PIN_4
#define DISP_4 PIN_5

uint16_t NUMBERS[] = {DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9};
uint16_t WINDOWS[] = {DISP_1, DISP_2, DISP_3, DISP_4};

#define MY_GPIO_MODER_MODE0 (0x3UL << (0U))
#define MY_GPIO_MODE (0x3uL << 0U)
#define MY_MODE_OUTPUT (0x1uL << 0u)
#define MY_MODE_AF (0x2uL << 0u)
#define MY_GPIO_OSPEEDR_OSPEED0 (0x3UL << (0U))
#define MY_GPIO_OTYPER_OT0 (0x1UL << (0U))
#define MY_OUTPUT_TYPE (0x1uL << 4u)
#define MY_OUTPUT_TYPE_Pos 4u
#define MY_MODE_ANALOG (0x3uL << 0u)
#define MY_GPIO_PUPDR_PUPD0 (0x3UL << (0U))
#define MY_GPIO_MODE_OUTPUT_PP ((0x1uL << 0u) | (0x0uL << 4u))
#define MY_GPIO_SPEED_LOW 0x00000000u
#define MY_GPIO_NOPULL 0x00000000u
#define MY_GPIO_MODE_INPUT (0x0uL << 0u)

typedef struct
{
  __IO uint32_t MODER;       /*!< GPIO port mode register, Address offset: 0x00 */
  __IO uint32_t OTYPER;      /*!< GPIO port output type register, Address offset: 0x04 */
  __IO uint32_t OSPEEDR;     /*!< GPIO port output speed register, Address offset: 0x08 */
  __IO uint32_t PUPDR;       /*!< GPIO port pull-up/pull-down register, Address offset: 0x0C */
  __IO uint32_t IDR;         /*!< GPIO port input data register, Address offset: 0x10 */
  __IO uint32_t ODR;         /*!< GPIO port output data register, Address offset: 0x14 */
  __IO uint32_t BSRR;        /*!< GPIO port bit set/reset register, Address offset: 0x18 */
  __IO uint32_t LCKR;        /*!< GPIO port configuration lock register, Address offset: 0x1C */
  __IO uint32_t AFR[2];      /*!< GPIO alternate function registers, Address offset: 0x20-0x24 */
  __IO uint32_t BRR;         /*!< GPIO Bit Reset register, Address offset: 0x28 */
} MY_GPIO_Struct;

typedef struct
{
  uint32_t Pin;        /*!< Specifies the GPIO pins to be configured. This parameter can be any value of @ref GPIO_pins */

  uint32_t Mode;       /*!< Specifies the operating mode for the selected pins. This parameter can be a value of @ref GPIO_mode */

  uint32_t Pull;       /*!< Specifies the Pull-up or Pull-Down activation for the selected pins. This parameter can be a value of @ref GPIO_pull */

  uint32_t Speed;      /*!< Specifies the speed for the selected pins. This parameter can be a value of @ref GPIO_speed */

  uint32_t Alternate;  /*!< Peripheral to be connected to the selected pins. This parameter can be a value of @ref GPIOEx_Alternate_function_selection */
} MY_GPIO_InitStruct;

#define MY_GPIOB ((MY_GPIO_Struct *)0x48000400)
#define MY_GPIOE ((MY_GPIO_Struct *)0x48001000)
#define MY_GPIOG ((MY_GPIO_Struct *)0x48001800)

#define JOY_BTN PIN_15

#define FREQUENCY 4000000u //4Mhz
#define TIM6_FREQ 1000000u
#define PSC_PERIOD (FREQUENCY / TIM6_FREQ - 1)
#define ARR_PERIOD 1000u - 1

#define DISPLAY_INTERVAL 1000
#define DECREMENT_INTERVAL 500
#define TIMER_DELAY 5

typedef volatile uint32_t reg32;
#define RCC_APB1 (volatile uint32_t *) 0x40021058 // RCC + 58 APB1

typedef struct TIM{ // nowy rejestr - obsługa czasu
	 reg32 TIM_CR1; // tryb pracy timera
	 reg32 TIM_CR2;
	 reg32 reserved1;
	 reg32 TIM_DIER;
	 reg32 TIM_SR; // rejestr statusu - flagi odnoszące się do uruchomienia zdarzeń
	 reg32 TIM_EGR; // rejestr generacji zdarzeń
	 reg32 reserved2[3];
	 reg32 TIM_CNT;
	 reg32 TIM_PSC; // preskaler, częstotliwość odliczania
	 reg32 TIM_ARR; // konfiguracja okresu

 }TIMS, *TIMSP;

#define TIM6_PTR ((TIMSP) 0x40001000)


 void MY_GPIO_Init(MY_GPIO_Struct *GPIOx, MY_GPIO_InitStruct *GPIO_Init);
 void init_display(void);
 void init_joystick(void);
 void display_single_digit(uint16_t num, uint16_t pin);
 void set_GPIOG_output(uint8_t pin);
 void set_GPIOE_input(uint8_t pin);
 void set_GPIOB_output(uint8_t pin);

 int OverflowDetection(void);
 void TIM_init(void);
 void IncrementTick(void);
 uint32_t GetTick(void) ;
 void Delay (unsigned ms);
 void decrement_counter(uint16_t *counter);
 void display_digits(uint8_t *digit_index);
 void num_math(uint16_t counter);
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
    HAL_Init();

    // SystemClock_Config();

    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOGEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOBEN);
    SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_GPIOEEN);
    SET_BIT(PWR->CR2, PWR_CR2_IOSV);
    init_display();
    init_joystick();
    TIM_init();


    uint8_t digit_index = 0;
    uint16_t counter = 0;
    int time_counter = 0;

    while (1)
    {
    	Delay(TIMER_DELAY);
    	        	if (time_counter % DECREMENT_INTERVAL == 0) {
    	        		 decrement_counter(&counter);
    	        		   num_math(counter);
    	        	}
    	        	if (time_counter % DISPLAY_INTERVAL == 0) {
    	        		display_digits( &digit_index);
    	        	}

    	        	time_counter+= TIMER_DELAY;


    }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */

int OverflowDetection()
{
	if(TIM6_PTR->TIM_SR & (1 << 0)) // sprawdza czy uruchomiło się zdarzenie upływu czasu
	{
		TIM6_PTR->TIM_SR &= 0; // zeruje flagę
		return 1;
	}
	return 0;
}

void TIM_init()
{
	*RCC_APB1 |= (1 << 4); // włączenie zegara dla timera TIM6
	TIM6_PTR->TIM_CR1 |= (1 << 7); // wlączenie trybu periodycznego i automatycznego przeładowania rejestru
	TIM6_PTR->TIM_PSC =  PSC_PERIOD;// konfiguracja prescalera
	TIM6_PTR->TIM_ARR =  ARR_PERIOD;// konfiguracja okresu timera
	TIM6_PTR->TIM_EGR |= (1 << 0); // ustawienie zdarzenia, aby po upływie okresu odliczało od 0
    // zdarzenie - ustawienie flagi w rejestrze statusu
	TIM6_PTR->TIM_CR1 |= (1 << 0);
}



void Delay (unsigned ms) {

		do{
			if (OverflowDetection()){
				ms--;
			}
		}
		while (ms>0);
}

void decrement_counter(uint16_t *counter)
{
        if ((MY_GPIOE->IDR & JOY_BTN) != 0x00u)
        {
            if (*counter == 10000)
            {
                *counter = 0;
            }
            else
            {
                (*counter)++;
            }
        }
        else
        {
            if (*counter == 0)
            {
                *counter = 9999;
            }
            else
            {
                (*counter)--;
            }
        }

}

uint8_t num1;
uint8_t num2;
uint8_t num3;
uint8_t num4;


void num_math(uint16_t counter)
{
    num1 = (counter / 1000) % 10;
    num2 = (counter / 100) % 10;
    num3 = (counter / 10) % 10;
    num4 = counter % 10;
}

void display_digits(uint8_t *digit_index)
{
    uint8_t digits[] = {num1, num2, num3, num4};


        display_single_digit(digits[*digit_index], *digit_index);
        *digit_index = (*digit_index + 1) % 4;

}

void init_display(void)
{
    RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN;
    PWR->CR1 |= PWR_CR1_VOS_0;
    SET_BIT(PWR->CR2, PWR_CR2_IOSV);

    MY_GPIO_InitStruct display_config;
    display_config.Pin = PIN_0 | PIN_1 | PIN_2 | PIN_3 | PIN_4 | PIN_5 | PIN_6;
    display_config.Mode = GPIO_MODE_OUTPUT_PP;
    display_config.Speed = GPIO_SPEED_FREQ_LOW;
    display_config.Pull = GPIO_NOPULL;

    MY_GPIO_Init(MY_GPIOG, &display_config);
    display_config.Pin = DISP_1 | DISP_2 | DISP_3 | DISP_4;

    MY_GPIO_Init(MY_GPIOB, &display_config);
}

void init_joystick(void)
{
    MY_GPIO_InitStruct joystick_config;
    joystick_config.Pin = JOY_BTN;
    joystick_config.Mode = GPIO_MODE_INPUT;
    joystick_config.Speed = GPIO_SPEED_FREQ_LOW;
    joystick_config.Pull = GPIO_NOPULL;

    MY_GPIO_Init(MY_GPIOE, &joystick_config);
}

void display_single_digit(uint16_t num, uint16_t pin)
{
    CLEAR_BIT(MY_GPIOB->ODR, PIN_2 | PIN_3 | PIN_4 | PIN_5);
    CLEAR_BIT(MY_GPIOG->ODR, DIGIT_8);
    SET_BIT(MY_GPIOB->ODR, WINDOWS[pin]);
    SET_BIT(MY_GPIOG->ODR, NUMBERS[num]);

}

void MY_GPIO_Init(MY_GPIO_Struct *GPIOx, MY_GPIO_InitStruct *GPIO_Init)
{
    uint32_t position = 0x00u;
    uint32_t iocurrent;
    uint32_t temp;

    /* Configure the port pins */
    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {
        /* Get current io position */
        iocurrent = (GPIO_Init->Pin) & (1uL << position);

        if (iocurrent != 0x00u)
        {
            /*--------------------- GPIO Mode Configuration ------------------------*/
            /* In case of Output or Alternate function mode selection */
            if (((GPIO_Init->Mode & MY_GPIO_MODE) == MY_MODE_OUTPUT) || ((GPIO_Init->Mode & MY_GPIO_MODE) == MY_MODE_AF))
            {
                /* Configure the IO Speed */
                temp = GPIOx->OSPEEDR;
                temp &= ~(GPIO_OSPEEDR_OSPEED0 << (position * 2u));
                temp |= (GPIO_Init->Speed << (position * 2u));
                GPIOx->OSPEEDR = temp;

                /* Configure the IO Output Type */
                temp = GPIOx->OTYPER;
                temp &= ~(GPIO_OTYPER_OT0 << position);
                temp |= (((GPIO_Init->Mode & MY_OUTPUT_TYPE) >> MY_OUTPUT_TYPE_Pos) << position);
                GPIOx->OTYPER = temp;
            }

#if defined(STM32L471xx) || defined(STM32L475xx) || defined(STM32L476xx) || defined(STM32L485xx) || defined(STM32L486xx)

            /* In case of Analog mode, check if ADC control mode is selected */
            if ((GPIO_Init->Mode & MY_MODE_ANALOG) == MY_MODE_ANALOG)
            {
                /* Configure the IO Output Type */
                temp = GPIOx->ASCR;
                temp &= ~(GPIO_ASCR_ASC0 << position);
                temp |= (((GPIO_Init->Mode & GPIO_MODE_ANALOG_ADC_CONTROL) >> 3) << position);
                GPIOx->ASCR = temp;
            }

#endif /* STM32L471xx || STM32L475xx || STM32L476xx || STM32L485xx || STM32L486xx */

            /* Activate the Pull-up or Pull down resistor for the current IO */
            if ((GPIO_Init->Mode & MY_GPIO_MODE) != MY_MODE_ANALOG)
            {
                /* Check the Pull parameter */
                assert_param(IS_GPIO_PULL(GPIO_Init->Pull));

                temp = GPIOx->PUPDR;
                temp &= ~(GPIO_PUPDR_PUPD0 << (position * 2U));
                temp |= ((GPIO_Init->Pull) << (position * 2U));
                GPIOx->PUPDR = temp;
            }

            /* In case of Alternate function mode selection */
            if ((GPIO_Init->Mode & MY_GPIO_MODE) == MY_MODE_AF)
            {
                /* Configure Alternate function mapped with the current IO */
                temp = GPIOx->AFR[position >> 3u];
                temp &= ~(0xFu << ((position & 0x07u) * 4u));
                temp |= ((GPIO_Init->Alternate) << ((position & 0x07u) * 4u));
                GPIOx->AFR[position >> 3u] = temp;
            }

            /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
            temp = GPIOx->MODER;
            temp &= ~(GPIO_MODER_MODE0 << (position * 2u));
            temp |= ((GPIO_Init->Mode & MY_GPIO_MODE) << (position * 2u));
            GPIOx->MODER = temp;

            /*--------------------- EXTI Mode Configuration ------------------------*/
            /* Configure the External Interrupt or event for the current IO */
            if ((GPIO_Init->Mode & EXTI_MODE) != 0x00u)
            {
                /* Enable SYSCFG Clock */
                __HAL_RCC_SYSCFG_CLK_ENABLE();

                temp = SYSCFG->EXTICR[position >> 2u];
                temp &= ~(0x0FuL << (4u * (position & 0x03u)));
                temp |= (GPIO_GET_INDEX(GPIOx) << (4u * (position & 0x03u)));
                SYSCFG->EXTICR[position >> 2u] = temp;

                /* Clear Rising Falling edge configuration */
                temp = EXTI->RTSR1;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & TRIGGER_RISING) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->RTSR1 = temp;

                temp = EXTI->FTSR1;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & TRIGGER_FALLING) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->FTSR1 = temp;

                /* Clear EXTI line configuration */
                temp = EXTI->EMR1;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & EXTI_EVT) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->EMR1 = temp;

                temp = EXTI->IMR1;
                temp &= ~(iocurrent);
                if ((GPIO_Init->Mode & EXTI_IT) != 0x00u)
                {
                    temp |= iocurrent;
                }
                EXTI->IMR1 = temp;
            }
        }

        position++;
    }
}


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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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

#ifdef USE_FULL_ASSERT
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
