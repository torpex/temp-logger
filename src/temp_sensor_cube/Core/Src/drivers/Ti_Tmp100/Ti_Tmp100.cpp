#include "Ti_Tmp100.hpp"

#define RAW_COUNTS_TO_DEG_C (0.0625)

// Constructor
Ti_Tmp100::Ti_Tmp100() {
    _i2cHandle = nullptr;
    _i2cAddress = 0;
}


ErrorStatus Ti_Tmp100::Init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress) {
    _i2cHandle = i2cHandle;
    _i2cAddress = i2cAddress;

    // Configure TMP100: 12-bit resolution
    uint8_t configData[2] = {0x01, 0x60};  // Register 0x01, 12-bit resolution
    if (HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress << 1, configData, 2, HAL_MAX_DELAY) != HAL_OK) {
        return ERROR;  // Failed to configure TMP100
    }

    return SUCCESS;  // Successfully configured TMP100
}

ErrorStatus Ti_Tmp100::ReadTemperature() {
    uint8_t tempReg = 0x00;  // Temperature register
    uint8_t buffer[2];

    // Request temperature data from TMP100
    if (HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress << 1, &tempReg, 1, HAL_MAX_DELAY) != HAL_OK) {
        return ERROR;
    }

    // Receive temperature data
    if (HAL_I2C_Master_Receive(_i2cHandle, _i2cAddress << 1, buffer, 2, HAL_MAX_DELAY) != HAL_OK) {
         return ERROR;
    }

    _rawTemperature = (buffer[0] << 8) | buffer[1];
    return SUCCESS;
}

int16_t Ti_Tmp100::GetTemperatureRawCounts() {
    return _rawTemperature;
}

float Ti_Tmp100::GetTemperatureDegC()
{
    return (float)_rawTemperature * RAW_COUNTS_TO_DEG_C;  // Convert to Celsius
}

float RawCountsToDegC(uint16_t rawCounts)
{
    return (float)rawCounts * RAW_COUNTS_TO_DEG_C;  // Convert to Celsius
}
