/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Canal_A_del_Encoder_Izquierdo_Pin GPIO_PIN_6
#define Canal_A_del_Encoder_Izquierdo_GPIO_Port GPIOA
#define Canal_A_del_Encoder_Izquierdo_EXTI_IRQn EXTI9_5_IRQn
#define output__motores_lado_izq_Pin GPIO_PIN_7
#define output__motores_lado_izq_GPIO_Port GPIOA
#define output__motores_lado_izqB10_Pin GPIO_PIN_10
#define output__motores_lado_izqB10_GPIO_Port GPIOB
#define Trigger_Pin GPIO_PIN_11
#define Trigger_GPIO_Port GPIOB
#define Echo_Pin GPIO_PIN_14
#define Echo_GPIO_Port GPIOB
#define pwm_izq_Pin GPIO_PIN_6
#define pwm_izq_GPIO_Port GPIOC
#define Canal_A_Encoder_Derecho_Pin GPIO_PIN_7
#define Canal_A_Encoder_Derecho_GPIO_Port GPIOC
#define Canal_A_Encoder_Derecho_EXTI_IRQn EXTI9_5_IRQn
#define pwm_derecho_Pin GPIO_PIN_8
#define pwm_derecho_GPIO_Port GPIOC
#define Canal_B_Enconder_Izquierdo_Pin GPIO_PIN_8
#define Canal_B_Enconder_Izquierdo_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define output__motores_lado_der_Pin GPIO_PIN_4
#define output__motores_lado_der_GPIO_Port GPIOB
#define output__motores_lado_derB5_Pin GPIO_PIN_5
#define output__motores_lado_derB5_GPIO_Port GPIOB
#define Canal_B_Encoder_Derecho_Pin GPIO_PIN_6
#define Canal_B_Encoder_Derecho_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
