#ifndef TMP100_HPP
#define TMP100_HPP

#include "stm32f4xx_hal.h"

class Ti_Tmp100 {
public:
    Ti_Tmp100();  // Constructor
    ErrorStatus Init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress);
    ErrorStatus ReadTemperature();
    int16_t GetTemperatureRaw();
    float GetTemperatureDegC();
    float ConvertRawTempToDegC(uint16_t rawCounts);

private:
    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _i2cAddress;
    int16_t _rawTemperature;
};

#endif // TMP100_HPP
