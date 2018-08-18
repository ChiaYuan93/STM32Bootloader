#ifndef __STM32F1xx_HAL_UART_H
#define __STM32F1xx_HAL_UART_H
#include "stm32f1xx_hal_def.h"

HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *huart);
HAL_StatusTypeDef HAL_UART_Transmit(uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef HAL_UART_Receive(uint8_t *pData, uint16_t Size);

#endif // __STM32F1xx_HAL_UART_H 




