#ifndef __stm32f1xx_hal_def_H
#define __stm32f1xx_hal_def_H
#include <stdint.h>

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

typedef struct UART_HandleTypeDef UART_HandleTypeDef;
struct UART_HandleTypeDef{
	
};

#endif // __stm32f1xx_hal_def_H 




