#ifndef _24FC_HPP
#define _24FC_HPP

#include "stm32f4xx_hal.h"

class Microchip_24FC {
public:
    Microchip_24FC();
    HAL_StatusTypeDef init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress);
    HAL_StatusTypeDef write(uint16_t memAddress, uint8_t* data, uint16_t size);
    HAL_StatusTypeDef read(uint16_t memAddress, uint8_t* data, uint16_t size);

private:
    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _i2cAddress;
};

#endif // _24FC_HPP