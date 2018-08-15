#ifndef __STM32F1xx_HAL_UART_H
#define __STM32F1xx_HAL_UART_H
#include "stm32f1xx_hal_def.h"

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

#endif // __STM32F1xx_HAL_UART_H 




