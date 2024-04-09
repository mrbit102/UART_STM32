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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "string.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int numer_przycisku = 0;

void zagas_diode();
void zapal_diode();

#define LINE_MAX_LENGTH	80

static char line_buffer[LINE_MAX_LENGTH + 1];
static uint32_t line_length;

int numer_diody;

void line_append(uint8_t value)
{
    // Sprawdzamy, czy odebrany znak to koniec linii
    if (value == '\r' || value == '\n') {
        // Odebrano znak końca linii
        if (line_length > 0) {
            // Dodajemy 0 na końcu linii
            line_buffer[line_length] = '\0';

            // Przetwarzamy dane
            if (strstr(line_buffer, "wlacz") != 0) {
            	if(strcmp(line_buffer, "wlacz all") == 0) {
            		HAL_GPIO_WritePin(LD10_GPIO_Port, GPIO_PIN_All, GPIO_PIN_SET);
            	}
            	// Jeżeli tytlko żadanie włączenia to pobiera numer z przycisku
            	if(strcmp(line_buffer, "wlacz") == 0) {
                    zapal_diode(numer_przycisku);
            	}
            	else {
                    // Jeżeli tekst zawiera "wlacz"
                    char *num_str = line_buffer + strlen("wlacz"); // Wskaźnik na początek numeru
                    numer_diody = atoi(num_str); // Konwertujemy tekst na liczbę
                    zapal_diode(numer_diody);
            	}

            } else if (strstr(line_buffer, "wylacz") != 0) {
            	if(strcmp(line_buffer, "wylacz all") == 0) {
            		HAL_GPIO_WritePin(LD10_GPIO_Port, GPIO_PIN_All, GPIO_PIN_RESET);
            	}
            	// Jeżeli tytlko żadanie włączenia to pobiera numer z przycisku
            	if(strcmp(line_buffer, "wylacz") == 0) {
                    zagas_diode(numer_przycisku);
            	}
            	else {
                    // Jeżeli tekst zawiera "wylacz"
                	char *num_str = line_buffer + strlen("wylacz"); // Wskaźnik na początek numeru
                	numer_diody = atoi(num_str); // Konwertujemy tekst na liczbę
                	// Przetwarzamy wyłączenie
                    zagas_diode(numer_diody);
            	}

            } else {
                printf("Nieznane polecenie: %s\n", line_buffer);
            }

            // Zaczynamy zbieranie danych od nowa
            line_length = 0;
        }
    } else {
        if (line_length >= LINE_MAX_LENGTH) {
            // Za dużo danych, usuwamy wszystko co odebraliśmy dotychczas
            line_length = 0;
        }
        // Dopisujemy wartość do bufora
        line_buffer[line_length++] = value;
    }
}

//void line_append(uint8_t value)
//{
//	if (value == '\r' || value == '\n') {
//		// odebraliśmy znak końca linii
//		if (line_length > 0) {
//			// dodajemy 0 na końcu linii
//			line_buffer[line_length] = '\0';
//			// przetwarzamy dane
//			if (strcmp(line_buffer, "wlacz") == 0) {
//				HAL_GPIO_WritePin(LD1_GPIO_Port, LD0_Pin, GPIO_PIN_SET);
//			} else if (strcmp(line_buffer, "wylacz") == 0) {
//				HAL_GPIO_WritePin(LD1_GPIO_Port, LD0_Pin, GPIO_PIN_RESET);
//			} else {
//				printf("Nieznane polecenie: %s\n", line_buffer);
//			}
//			// zaczynamy zbieranie danych od nowa
//			line_length = 0;
//		}
//	}
//	else {
//		if (line_length >= LINE_MAX_LENGTH) {
//			// za dużo danych, usuwamy wszystko co odebraliśmy dotychczas
//			line_length = 0;
//		}
//		// dopisujemy wartość do bufora
//		line_buffer[line_length++] = value;
//	}
//}

int __io_putchar(int ch)
{
    if (ch == '\n') {
        uint8_t ch2 = '\r';
        HAL_UART_Transmit(&huart2, &ch2, 1, HAL_MAX_DELAY);
    }

    HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return 1;
}

typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
}pin_t;


static const pin_t LED[] = {
		{ LD1_GPIO_Port, LD0_Pin },
		{ LD1_GPIO_Port, LD1_Pin },
		{ LD1_GPIO_Port, LD8_Pin },
		{ LD1_GPIO_Port, LD9_Pin },
		{ LD1_GPIO_Port, LD4_Pin },
		{ LD1_GPIO_Port, LD5_Pin },
		{ LD1_GPIO_Port, LD6_Pin },
		{ LD1_GPIO_Port, LD7_Pin },
		{ LD1_GPIO_Port, LD10_Pin },
		{ LD1_GPIO_Port, LD11_Pin },
};

void zapal_diode(int numer_diody){

	GPIO_PinState state = GPIO_PIN_SET;

	int led = numer_diody;

	if (led >= 0 && led < 10) {
		HAL_GPIO_WritePin(LED[led].port, LED[led].pin, state);
        }
}

void zagas_diode(int numer_diody){

	GPIO_PinState state = GPIO_PIN_RESET;

	int led = numer_diody;

	if (led >= 0 && led < 10) {
		HAL_GPIO_WritePin(LED[led].port, LED[led].pin, state);
        }
}


void led_set(int led, bool turn_on)
{
	GPIO_PinState state = (turn_on) ? GPIO_PIN_SET : GPIO_PIN_RESET;

	if (led >= 0 && led < 10) {
		HAL_GPIO_WritePin(LED[led].port, LED[led].pin, state);
        }
}

void send_uart(const char *text) {
    HAL_UART_Transmit(&huart2, (uint8_t *)text, strlen(text), HAL_MAX_DELAY);
}


bool is_button_pressed(void) {
  if (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET ) {
    return true;
  } else {
    return false;
  }
}

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
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  printf("Hello world! ");
  printf("Hello world! ");
  printf("Hello world! ");
  fflush(stdout);

  float pi = 3.14f;
  printf("\nLiczba pi to: %f\n", pi);

  while (1)
  {
	  uint8_t value;
	  if (HAL_UART_Receive(&huart2, &value, 1, 0) == HAL_OK)
		  line_append(value);
	  if(HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin) == GPIO_PIN_RESET ){
		  numer_przycisku++;
		  if (numer_przycisku >= 10)
			  numer_przycisku = 0;
//		  char przycisk[50]; // Załóżmy maksymalną długość tekstu jako 50

		  printf("Ilosc wcisnienc wynosi: %d\n", numer_przycisku);

	      while(is_button_pressed())
	      {}
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_RXOVERRUNDISABLE_INIT;
  huart2.AdvancedInit.OverrunDisable = UART_ADVFEATURE_OVERRUN_DISABLE;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD0_Pin|LD1_Pin|LD4_Pin|LD5_Pin
                          |LD6_Pin|LD7_Pin|LD8_Pin|LD9_Pin
                          |LD10_Pin|LD11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BTN_Pin */
  GPIO_InitStruct.Pin = BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD0_Pin LD1_Pin LD4_Pin LD5_Pin
                           LD6_Pin LD7_Pin LD8_Pin LD9_Pin
                           LD10_Pin LD11_Pin */
  GPIO_InitStruct.Pin = LD0_Pin|LD1_Pin|LD4_Pin|LD5_Pin
                          |LD6_Pin|LD7_Pin|LD8_Pin|LD9_Pin
                          |LD10_Pin|LD11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
