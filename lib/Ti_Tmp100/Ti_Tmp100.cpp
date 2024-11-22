#include "Ti_Tmp100.hpp"

// Constants
#define RAW_COUNTS_TO_DEG_C (0.0625)

// TMP100 register addresses
#define TEMP_REG   (0x00)
#define CONFIG_REG (0x01)

// Config register bits
#define SD  (1<<0) // Shutdown mode
#define TM  (1<<1) // Thermostat mode (N/A for TMP100)
#define POL (1<<2) // Polarity (N/A for TMP100)
#define F0  (1<<3) // Fault queue bit 0
#define F1  (1<<3) // Fault queue bit 1
#define R0  (1<<5) // Resolution bit 0
#define R1  (1<<6) // Resolution bit 1
#define OS  (1<<7) // One-shot mode

// Constructor
Ti_Tmp100::Ti_Tmp100() {
    _i2cHandle = nullptr;
    _i2cAddress = 0;
    _rawTemperature = 0;
}

// Initialize TMP100 device
// Inputs: I2C handle and device address
// Outputs: ErrorStatus (SUCCESS or ERROR)
ErrorStatus Ti_Tmp100::Init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress) {
    _i2cHandle = i2cHandle;
    _i2cAddress = i2cAddress << 1; // Shift address left by 1 to make room for R/W bit

    // Configure TMP100 for 12-bit resolution
    uint8_t configData[2];
    configData[0] = CONFIG_REG;
    configData[1] = (R0 | R1);  // 12-bit resolution
    if (HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, configData, sizeof(configData), HAL_MAX_DELAY) != HAL_OK) {
        return ERROR;  // Failed to configure TMP100
    }

    return SUCCESS;  // Successfully configured TMP100
}

// Read temperature from TMP100
// Inputs: None
// Outputs: Temperature in _rawTemperature, ErrorStatus (SUCCESS or ERROR)
ErrorStatus Ti_Tmp100::ReadTemperature() {
    uint8_t tempReg = TEMP_REG;  // Temperature register
    uint8_t buffer[2];

    // Request temperature data from TMP100
    if (HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, &tempReg, 1, HAL_MAX_DELAY) != HAL_OK) {
        return ERROR;
    }

    // Receive temperature data
    if (HAL_I2C_Master_Receive(_i2cHandle, _i2cAddress, buffer, sizeof(buffer), HAL_MAX_DELAY) != HAL_OK) {
         return ERROR;
    }

    // Combine MSB and LSB into 12-bit raw temperature and right shift by 4 bits adjust for unused bits
    _rawTemperature = ((buffer[0] << 8) | buffer[1]) >> 4;
    return SUCCESS;
}

// Get last temperature measurement in raw counts
// Inputs: None
// Outputs: Raw temperature counts from last measurement (0 if no previous measurement)
int16_t Ti_Tmp100::GetTemperatureRaw() {
    return _rawTemperature;
}

// Get last temperature measurement in degC (useful if reading temperature directly from device)
// Inputs: None
// Outputs: Raw temperature counts from last measurement (0 if no previous measurement)
float Ti_Tmp100::GetTemperatureDegC()
{
    return ConvertRawTempToDegC(_rawTemperature);
}

// Convert raw temperature counts to degC (useful if reading raw temperature value from a file)
// Inputs: rawTemperature - raw temperature counts from TMP100
// Outputs: Temperature in degC
float Ti_Tmp100::ConvertRawTempToDegC(uint16_t rawTemperature)
{
    return (float)rawTemperature * RAW_COUNTS_TO_DEG_C;  // Convert to Celsius
}
