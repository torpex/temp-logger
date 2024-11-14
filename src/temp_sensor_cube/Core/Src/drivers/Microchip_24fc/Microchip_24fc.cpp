#include "Microchip_24fc.hpp"
#include <cstring>

Microchip_24FC::Microchip_24FC() {
    _i2cHandle = nullptr;
    _i2cAddress = 0;
}

HAL_StatusTypeDef Microchip_24FC::init(I2C_HandleTypeDef* i2cHandle, uint8_t i2cAddress) {
    _i2cHandle = i2cHandle;
    _i2cAddress = i2cAddress;
    return HAL_OK;
}

HAL_StatusTypeDef Microchip_24FC::write(uint16_t memAddress, uint8_t* data, uint16_t size) {
    uint8_t buffer[2 + size];
    buffer[0] = (memAddress >> 8) & 0xFF; // MSB of memory address
    buffer[1] = memAddress & 0xFF;        // LSB of memory address
    memcpy(&buffer[2], data, size);

    return HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, buffer, sizeof(buffer), HAL_MAX_DELAY);
}

HAL_StatusTypeDef Microchip_24FC::read(uint16_t memAddress, uint8_t* data, uint16_t size) {
    uint8_t address[2];
    address[0] = (memAddress >> 8) & 0xFF; // MSB of memory address
    address[1] = memAddress & 0xFF;        // LSB of memory address

    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(_i2cHandle, _i2cAddress, address, sizeof(address), HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status;
    }

    return HAL_I2C_Master_Receive(_i2cHandle, _i2cAddress, data, size, HAL_MAX_DELAY);
}
