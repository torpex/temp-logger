#include "Microchip_24fc.hpp"
#include <cstring>

Microchip_24FC::Microchip_24FC() {
    _i2cHandle = nullptr;
    _i2cAddress = 0;
}

ErrorStatus Microchip_24FC::Init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress, uint32_t size) {
    _i2cHandle = i2cHandle;
    _i2cAddress = i2cAddress;
    _deviceSize = size;
    return SUCCESS;
}

ErrorStatus Microchip_24FC::Write(uint16_t address, uint8_t* data, uint16_t size) {
    if (NULL == data) {
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

ErrorStatus Microchip_24FC::Read(uint16_t address, uint8_t* data, uint16_t size) {
    if (NULL == data) {
        return ERROR;
    }

    uint8_t i2cAddr[2];
    i2cAddr[0] = (address >> 8) & 0xFF; // MSB of memory address
    i2cAddr[1] = address & 0xFF;        // LSB of memory address

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, i2cAddr, sizeof(address), HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return ERROR;
    }

    status = HAL_I2C_Master_Receive(_i2cHandle, _i2cAddress, data, size, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return ERROR;
    }

    return SUCCESS;
}

uint32_t Microchip_24FC::GetSize()
{
    return _deviceSize;
}