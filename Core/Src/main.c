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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "VL53L1X.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	int16_t velocity;
	int64_t position;
	uint32_t lastCounterValue;
} encoder;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3

#define R_THRESH 200
#define M_THRESH 500
#define L_THRESH 200

#define Write HAL_GPIO_WritePin
#define Read HAL_GPIO_ReadPin
#define Delay HAL_Delay

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// Sensor variables
VL53L1X left, middle, right;
VL53L1X* sensors[] = {&left, &middle, &right};
uint32_t distanceL, distanceM, distanceR;

// Encoder structs for the "legs"
encoder legA, legB;

// Variable to remember previous location of enemy
uint8_t previous = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// Initializes the motors, sensors, LED and encoders to make things neater
void InitPeriph(void);

// Function i found on the net to update the encoder struct's position and velocity variables
void UpdateEncoder(encoder *encoderValue, TIM_HandleTypeDef *htim);

// Turning function
void Turn(uint8_t direction, uint16_t speed, uint16_t amount);

// Driving function
void Drive(uint8_t direction, uint16_t speed, uint16_t amount);

// Updates sensor variables in one neat little package
void GetDistances(void);

// Spins the hands at a given speed
void SpinHands(uint16_t speed);

// Stops the hands
void StopHands(void);

// Regulates the RGB led values (1 is off, 0 is on)
void LED(uint8_t red, uint8_t green, uint8_t blue);

// Writing function so that you can use printf() with UART
int _write(int file, char *ptr, int len);

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
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  InitPeriph();

  // Wait for start command
  LED(1, 0, 1);
  while (!Read(START_GPIO_Port, START_Pin)) {}
  LED(1, 1, 1);

  // Little jerk so that the arms can fall down into the active position
  Drive(FORWARD, 50, 100);
  Drive(BACKWARD, 50, 100);

  // Start the "weapon" spin
  SpinHands(50);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (!Read(START_GPIO_Port, START_Pin)) {
		  LED(0,1,1);
		  StopHands();
	  }
	  else {
beginning:
		  SpinHands(50);
		  GetDistances();

		  if (!Read(LINE1_GPIO_Port, LINE1_Pin)) {
			  Drive(BACKWARD, 50, 1000);
			  Turn(LEFT, 50, 1000);
			  goto beginning;
		  }
		  else if (!Read(LINE2_GPIO_Port, LINE2_Pin)) {
			  Drive(FORWARD, 50, 1000);
			  Turn(RIGHT, 50, 1000);
			  goto beginning;
		  }

		  if (distanceM < M_THRESH) {
			  LED(1, 0, 1); // Green
			  Drive(FORWARD, 50, 1000);
			  previous = 1;
		  }
		  if (distanceL < L_THRESH) {
			  LED(0, 1, 1); // Red
			  Turn(LEFT, 50, 1000);
			  previous = 2;
		  }
		  if (distanceR < R_THRESH){
			  LED(1, 1, 0); // Blue
			  Turn(RIGHT, 50, 1000);
			  previous = 3;
		  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
/*
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_9) {
		LED(0, 0, 1);
		//StopHands();
		// Drive(FORWARD, 50, 50);
		Delay(500);
		//SpinHands();
		//LED(1,1,1);
	}
	if (GPIO_Pin == GPIO_PIN_15) {
		LED(1, 0, 0);
		//StopHands();
		// Drive(BACKWARD, 50, 50);
		Delay(500);
		//SpinHands();
		//LED(1,1,1);
	}
}*/

// Initializes all the required peripherals for Duubel V3
void InitPeriph(void) {
	// Motor PWM channels
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0);

	// Hand motor direction
	Write(HA_IN1_GPIO_Port, HA_IN1_Pin, GPIO_PIN_RESET); //Right hand
	Write(HA_IN2_GPIO_Port, HA_IN2_Pin, GPIO_PIN_SET);

	Write(HB_IN1_GPIO_Port, HB_IN1_Pin, GPIO_PIN_RESET); //Left hand
	Write(HB_IN2_GPIO_Port, HB_IN2_Pin, GPIO_PIN_SET);

	// Motor encoders
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);

	// TOF sensors
	TOF_InitStruct(&right, &hi2c2, 0x30, XSHUT1_GPIO_Port, XSHUT1_Pin);
	TOF_InitStruct(&left, &hi2c2, 0x32, XSHUT2_GPIO_Port, XSHUT2_Pin);
	TOF_InitStruct(&middle, &hi2c2, 0x34, XSHUT3_GPIO_Port, XSHUT3_Pin);
	TOF_BootMultipleSensors(sensors, 3);

	// Turn the debug LED completely off
	Write(RED_GPIO_Port, RED_Pin, 1);
	Write(BLUE_GPIO_Port, BLUE_Pin, 1);
	Write(GREEN_GPIO_Port, GREEN_Pin, 1);
}

// Updates encoder structs
void UpdateEncoder(encoder *encoderValue, TIM_HandleTypeDef *htim) {
	uint32_t temp_counter = __HAL_TIM_GET_COUNTER(htim);
	static uint8_t first_time = 0;
	if(!first_time) {
		encoderValue->velocity = 0;
		first_time = 1;
	}
	else {
		if(temp_counter == encoderValue ->lastCounterValue) {
			encoderValue->velocity = 0;
		}
		else if(temp_counter > encoderValue ->lastCounterValue) {
			if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)) {
				encoderValue->velocity = -encoderValue->lastCounterValue - (__HAL_TIM_GET_AUTORELOAD(htim)-temp_counter);
			}
			else {
				encoderValue->velocity = temp_counter - encoderValue->lastCounterValue;
			}
		}
		else {
			if (__HAL_TIM_IS_TIM_COUNTING_DOWN(htim)) {
				encoderValue->velocity = temp_counter - encoderValue->lastCounterValue;
			}
			else {
				encoderValue->velocity = temp_counter + (__HAL_TIM_GET_AUTORELOAD(htim) - encoderValue->lastCounterValue);
			}
		}
	}
	encoderValue->position += encoderValue->velocity;
	encoderValue->lastCounterValue = temp_counter;
}

// Allows the robot to turn in place
void Turn(uint8_t direction, uint16_t speed, uint16_t amount) {
	int64_t previousA = legA.position;
	int64_t previousB = legB.position;

	if (direction == RIGHT) { // Turn Right
		// CW setup
		Write(LA_IN1_GPIO_Port, LA_IN1_Pin, GPIO_PIN_SET);
		Write(LA_IN2_GPIO_Port, LA_IN2_Pin, GPIO_PIN_RESET);

		// CCW setup
		Write(LB_IN1_GPIO_Port, LB_IN1_Pin, GPIO_PIN_SET);
		Write(LB_IN2_GPIO_Port, LB_IN2_Pin, GPIO_PIN_RESET);

		// Speed set
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed); // LB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, speed); // LA

		while ((legA.position < previousA + amount) || (legB.position > previousB - amount)) {
			UpdateEncoder(&legA, &htim3);
			UpdateEncoder(&legB, &htim4);
		}

		// Speed reset
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0); // LB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0); // LA
	}
	else { // Turn Left
		// CCW setup
		Write(LA_IN1_GPIO_Port, LA_IN1_Pin, GPIO_PIN_RESET);
		Write(LA_IN2_GPIO_Port, LA_IN2_Pin, GPIO_PIN_SET);

		// CW setup
		Write(LB_IN1_GPIO_Port, LB_IN1_Pin, GPIO_PIN_RESET);
		Write(LB_IN2_GPIO_Port, LB_IN2_Pin, GPIO_PIN_SET);

		// Speed set
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed); // HB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, speed); // HA

		while ((legA.position > previousA - amount) || (legB.position < previousB + amount)) {
			UpdateEncoder(&legA, &htim3);
			UpdateEncoder(&legB, &htim4);
		}

		// Speed reset
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0); // HB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0); // HA
	}
}

// Drives forward or backward the given amount
void Drive(uint8_t direction, uint16_t speed, uint16_t amount) {
	int64_t previousA = legA.position;
	int64_t previousB = legB.position;

	if (direction == FORWARD) { // Forward
		// CW setup
		Write(LA_IN1_GPIO_Port, LA_IN1_Pin, GPIO_PIN_SET);
		Write(LA_IN2_GPIO_Port, LA_IN2_Pin, GPIO_PIN_RESET);

		// CW setup
		Write(LB_IN1_GPIO_Port, LB_IN1_Pin, GPIO_PIN_RESET);
		Write(LB_IN2_GPIO_Port, LB_IN2_Pin, GPIO_PIN_SET);

		// Speed set
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed); // LB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, speed); // LA

		while ((legA.position < previousA + amount) || (legB.position < previousB + amount)) {
			UpdateEncoder(&legA, &htim3);
			UpdateEncoder(&legB, &htim4);
		}

		// Speed reset
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0); // LB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0); // LA
	}
	else { // Backward
		// CCW setup
		Write(LA_IN1_GPIO_Port, LA_IN1_Pin, GPIO_PIN_RESET);
		Write(LA_IN2_GPIO_Port, LA_IN2_Pin, GPIO_PIN_SET);

		// CCW setup
		Write(LB_IN1_GPIO_Port, LB_IN1_Pin, GPIO_PIN_SET);
		Write(LB_IN2_GPIO_Port, LB_IN2_Pin, GPIO_PIN_RESET);

		// Speed set
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, speed); // HB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, speed); // HA

		while ((legA.position > previousA - amount) || (legB.position > previousB - amount)) {
			UpdateEncoder(&legA, &htim3);
			UpdateEncoder(&legB, &htim4);
		}

		// Speed reset
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0); // HB
		__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0); // HA
	}
}

// Updates all distances
void GetDistances(void) {
	distanceR = TOF_GetDistance(&right);
	distanceM = TOF_GetDistance(&middle);
	distanceL = TOF_GetDistance(&left);
}

int _write(int file, char* ptr, int len) {
	HAL_UART_Transmit(&huart1, ptr, len, 1000);
	return len;
}

void SpinHands(uint16_t speed) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, speed);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, speed);
}

void StopHands(void) {
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
	__HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
}

void LED(uint8_t red, uint8_t green, uint8_t blue) {
	Write(RED_GPIO_Port, RED_Pin, red);
	Write(GREEN_GPIO_Port, GREEN_Pin, green);
	Write(BLUE_GPIO_Port, BLUE_Pin, blue);
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
