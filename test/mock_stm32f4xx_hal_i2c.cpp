#include "mock_stm32f4xx_hal_i2c.hpp"

MockHAL_I2C* mockHAL_I2C = nullptr;

extern "C" {
    HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef* hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout) {
        return mockHAL_I2C->HAL_I2C_Master_Transmit(hi2c, DevAddress, pData, Size, Timeout);
    }

    HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef* hi2c, uint16_t DevAddress, uint8_t* pData, uint16_t Size, uint32_t Timeout) {
        return mockHAL_I2C->HAL_I2C_Master_Receive(hi2c, DevAddress, pData, Size, Timeout);
    }
}
