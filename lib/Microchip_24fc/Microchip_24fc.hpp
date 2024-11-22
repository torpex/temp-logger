#ifndef _24FC_HPP
#define _24FC_HPP

#include "stm32f4xx_hal.h"

#define EEPROM_BYTES_PER_PAGE (256)

class Microchip_24FC {
public:
    Microchip_24FC();
    ErrorStatus Init(I2C_HandleTypeDef* _i2cHandle, uint8_t i2c_address, uint32_t size);
    ErrorStatus Write(uint16_t memAddress, uint8_t* data, uint16_t size);
    ErrorStatus Read(uint16_t memAddress, uint8_t* data, uint16_t size);
    uint32_t GetSize();

private:
    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _i2cAddress;
    uint32_t _eepromSize;
};

#endif // _24FC_HPP
