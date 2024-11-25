#ifndef MOCK_STM32F4XX_HAL_I2C_H
#define MOCK_STM32F4XX_HAL_I2C_H

#include <gmock/gmock.h>
#include "stm32f4xx_hal.h"

class MockHAL_I2C {
public:
    MOCK_METHOD(HAL_StatusTypeDef, HAL_I2C_Master_Transmit, (I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t));
    MOCK_METHOD(HAL_StatusTypeDef, HAL_I2C_Master_Receive, (I2C_HandleTypeDef*, uint16_t, uint8_t*, uint16_t, uint32_t));
};

extern MockHAL_I2C* mockHAL_I2C;

extern "C" {
    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef* hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout);
    HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef* hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout);
}

#endif // MOCK_STM32F4XX_HAL_I2C_H