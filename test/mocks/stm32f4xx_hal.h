#ifndef STM32F4XX_HAL_H
#define STM32F4XX_HAL_H

#include <stdint.h>

typedef struct {
    int dummy; // Add a dummy member to ensure consistent struct size between C and C++
} I2C_HandleTypeDef;

typedef enum {
    HAL_OK,
    HAL_ERROR,
    HAL_BUSY,
    HAL_TIMEOUT
} HAL_StatusTypeDef;

typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;

#define I2C_MEMADD_SIZE_8BIT 0x0001U
#define HAL_MAX_DELAY 0xFFFFFFFFU

// Mock function prototypes
// Extern "C" to prevent name mangling
extern "C" {
    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
}

#endif // STM32F4XX_HAL_H