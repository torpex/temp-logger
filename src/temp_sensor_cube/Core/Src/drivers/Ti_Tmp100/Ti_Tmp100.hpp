#ifndef TMP100_HPP
#define TMP100_HPP

#include "stm32f4xx_hal.h"

class Ti_Tmp100 {
public:
    Ti_Tmp100();  // Constructor
    bool init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress);
    float readTemperature();

private:
    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _i2cAddress;
};

#endif // TMP100_HPP
