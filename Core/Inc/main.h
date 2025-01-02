/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RED_Pin GPIO_PIN_13
#define RED_GPIO_Port GPIOC
#define GREEN_Pin GPIO_PIN_14
#define GREEN_GPIO_Port GPIOC
#define BLUE_Pin GPIO_PIN_15
#define BLUE_GPIO_Port GPIOC
#define OSC_IN_Pin GPIO_PIN_0
#define OSC_IN_GPIO_Port GPIOD
#define OSC_OUT_Pin GPIO_PIN_1
#define OSC_OUT_GPIO_Port GPIOD
#define HB_PWM_Pin GPIO_PIN_0
#define HB_PWM_GPIO_Port GPIOA
#define HA_PWM_Pin GPIO_PIN_1
#define HA_PWM_GPIO_Port GPIOA
#define LB_PWM_Pin GPIO_PIN_2
#define LB_PWM_GPIO_Port GPIOA
#define LA_PWM_Pin GPIO_PIN_3
#define LA_PWM_GPIO_Port GPIOA
#define LA_IN1_Pin GPIO_PIN_4
#define LA_IN1_GPIO_Port GPIOA
#define LA_IN2_Pin GPIO_PIN_5
#define LA_IN2_GPIO_Port GPIOA
#define ENC_A1_Pin GPIO_PIN_6
#define ENC_A1_GPIO_Port GPIOA
#define ENC_A2_Pin GPIO_PIN_7
#define ENC_A2_GPIO_Port GPIOA
#define HA_IN1_Pin GPIO_PIN_0
#define HA_IN1_GPIO_Port GPIOB
#define HA_IN2_Pin GPIO_PIN_1
#define HA_IN2_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_10
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_11
#define SDA_GPIO_Port GPIOB
#define XSHUT3_Pin GPIO_PIN_12
#define XSHUT3_GPIO_Port GPIOB
#define XSHUT2_Pin GPIO_PIN_13
#define XSHUT2_GPIO_Port GPIOB
#define XSHUT1_Pin GPIO_PIN_14
#define XSHUT1_GPIO_Port GPIOB
#define LINE2_Pin GPIO_PIN_15
#define LINE2_GPIO_Port GPIOB
#define LINE1_Pin GPIO_PIN_8
#define LINE1_GPIO_Port GPIOA
#define U_TX_Pin GPIO_PIN_9
#define U_TX_GPIO_Port GPIOA
#define U_RX_Pin GPIO_PIN_10
#define U_RX_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define START_Pin GPIO_PIN_15
#define START_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define HB_IN2_Pin GPIO_PIN_4
#define HB_IN2_GPIO_Port GPIOB
#define HB_IN1_Pin GPIO_PIN_5
#define HB_IN1_GPIO_Port GPIOB
#define ENC_B2_Pin GPIO_PIN_6
#define ENC_B2_GPIO_Port GPIOB
#define ENC_B1_Pin GPIO_PIN_7
#define ENC_B1_GPIO_Port GPIOB
#define LB_IN2_Pin GPIO_PIN_8
#define LB_IN2_GPIO_Port GPIOB
#define LB_IN1_Pin GPIO_PIN_9
#define LB_IN1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
