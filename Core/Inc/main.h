/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "ZJY/ZJY.h"

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
#define KEY_F_START_Pin GPIO_PIN_0
#define KEY_F_START_GPIO_Port GPIOA
#define LED_F_Pin GPIO_PIN_2
#define LED_F_GPIO_Port GPIOA
#define pwm_Pin GPIO_PIN_3
#define pwm_GPIO_Port GPIOA
#define LED_MIT_Pin GPIO_PIN_4
#define LED_MIT_GPIO_Port GPIOA
#define ADC_TIM_Pin GPIO_PIN_5
#define ADC_TIM_GPIO_Port GPIOA
#define ADC_F_Pin GPIO_PIN_6
#define ADC_F_GPIO_Port GPIOA
#define BEEP_Pin GPIO_PIN_11
#define BEEP_GPIO_Port GPIOB
#define LED_LX_Pin GPIO_PIN_3
#define LED_LX_GPIO_Port GPIOB
#define KEY_MODE_Pin GPIO_PIN_4
#define KEY_MODE_GPIO_Port GPIOB
#define LED_DX_Pin GPIO_PIN_5
#define LED_DX_GPIO_Port GPIOB
#define LED_SM_Pin GPIO_PIN_9
#define LED_SM_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
