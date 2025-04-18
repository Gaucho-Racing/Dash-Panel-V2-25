/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32u5xx_hal.h"

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
#define LED1G_Pin GPIO_PIN_9
#define LED1G_GPIO_Port GPIOG
#define LCD_DISP_RESET_Pin GPIO_PIN_7
#define LCD_DISP_RESET_GPIO_Port GPIOH
#define CTP_RST_Pin GPIO_PIN_3
#define CTP_RST_GPIO_Port GPIOE
#define IMD_Pin GPIO_PIN_3
#define IMD_GPIO_Port GPIOG
#define CTP_INT_Pin GPIO_PIN_6
#define CTP_INT_GPIO_Port GPIOE
#define CTP_INT_EXTI_IRQn EXTI6_IRQn
#define USB_PWR_EN_Pin GPIO_PIN_8
#define USB_PWR_EN_GPIO_Port GPIOG
#define USB_OVERCURRENT_Pin GPIO_PIN_0
#define USB_OVERCURRENT_GPIO_Port GPIOJ
#define BSPD_Pin GPIO_PIN_4
#define BSPD_GPIO_Port GPIOG
#define LED1R_Pin GPIO_PIN_2
#define LED1R_GPIO_Port GPIOG
#define R_RST_Pin GPIO_PIN_4
#define R_RST_GPIO_Port GPIOF
#define AMS_Pin GPIO_PIN_11
#define AMS_GPIO_Port GPIOH
#define LED2R_Pin GPIO_PIN_4
#define LED2R_GPIO_Port GPIOA
#define LED2B_Pin GPIO_PIN_1
#define LED2B_GPIO_Port GPIOB
#define R_CS_Pin GPIO_PIN_1
#define R_CS_GPIO_Port GPIOG
#define LED1B_Pin GPIO_PIN_3
#define LED1B_GPIO_Port GPIOA
#define RTD_Pin GPIO_PIN_6
#define RTD_GPIO_Port GPIOA
#define TS_ACTIVE_Pin GPIO_PIN_7
#define TS_ACTIVE_GPIO_Port GPIOA
#define R_INT_Pin GPIO_PIN_4
#define R_INT_GPIO_Port GPIOC
#define LED2G_Pin GPIO_PIN_15
#define LED2G_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
// See Brand Guidelines
#define GR_COLOR_PINK 0xEF0DA1
#define GR_COLOR_PURPLE 0x7920FF
#define GR_COLOR_NAVY 0x195297
#define GR_COLOR_GRAY 0x9AA3B0
#define GR_COLOR_WHITE 0xFFFFFF
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
