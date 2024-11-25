#ifndef _24FC_HPP
#define _24FC_HPP

#include "stm32f4xx_hal.h"

#define EEPROM_BYTES_PER_PAGE (64)

class Microchip_24FC {
public:
    Microchip_24FC();
    virtual ErrorStatus Init(I2C_HandleTypeDef* _i2cHandle, uint8_t i2c_address, uint32_t size);
    virtual ErrorStatus Write(uint32_t memAddress, uint8_t* data, uint16_t size);
    virtual ErrorStatus Read(uint32_t memAddress, uint8_t* data, uint16_t size);
    virtual uint32_t GetSize();

private:
    I2C_HandleTypeDef* _i2cHandle;
    uint8_t _i2cAddress;
    uint32_t _eepromSize;
};

#endif // _24FC_HPP
