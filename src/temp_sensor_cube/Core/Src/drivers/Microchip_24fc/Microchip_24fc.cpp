#include "Microchip_24fc.hpp"
#include <cstring>

#define MAX_WRITE_SIZE_BYTES (64)

// Constructor
Microchip_24FC::Microchip_24FC() {
    _i2cHandle = nullptr;
    _i2cAddress = 0;
    _eepromSize = 0;
}

// Initialize EEPROM device
// Inputs: I2C handle, device address, and size of the EEPROM
// Outputs: ErrorStatus (SUCCESS or ERROR)
ErrorStatus Microchip_24FC::Init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress, uint32_t size) {
    _i2cHandle = i2cHandle;
    _i2cAddress = i2cAddress;
    _eepromSize = size;
    return SUCCESS;
}

// Write data to EEPROM (max 64 bytes at a time)
// Inputs: Memory address, data buffer, and size of data
// Outputs: ErrorStatus (SUCCESS or ERROR)
ErrorStatus Microchip_24FC::Write(uint16_t address, uint8_t* data, uint16_t size) {
    if (NULL == data || size > MAX_WRITE_SIZE_BYTES) {
        return ERROR;
    }

    uint8_t buffer[2 + size];
    buffer[0] = (address >> 8) & 0xFF; // MSB of memory address
    buffer[1] = address & 0xFF;        // LSB of memory address
    memcpy(&buffer[2], data, size);

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, buffer, sizeof(buffer), HAL_MAX_DELAY);
    if (status != HAL_OK)
    {
        return ERROR;
    }

    return SUCCESS;
}

// Read data from EEPROM (max 64 bytes at a time)
// Inputs: Memory address, target data pointer, and size of data
// Outputs: ErrorStatus (SUCCESS or ERROR), read contents in data buffer
ErrorStatus Microchip_24FC::Read(uint16_t address, uint8_t* data, uint16_t size) {
    if (NULL == data || size > MAX_WRITE_SIZE_BYTES) {
        return ERROR;
    }

    // Load EEPROM address into buffer
    uint8_t buffer[2];
    buffer[0] = (address >> 8) & 0xFF; // MSB of memory address
    buffer[1] = address & 0xFF;        // LSB of memory address

    // Request data from EEPROM
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, buffer, sizeof(buffer), HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return ERROR;
    }

    // Read data from EEPROM
    status = HAL_I2C_Master_Receive(_i2cHandle, _i2cAddress, data, size, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return ERROR;
    }

    return SUCCESS;
}

// Get size of EEPROM in bytes
// Inputs: none
// Outputs: Size of EEPROM in bytes (if initialized, otherwise 0)
uint32_t Microchip_24FC::GetSize()
{
    return _eepromSize;
}
