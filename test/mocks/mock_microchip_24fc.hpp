#ifndef MOCK_MICROCHIP_24FC_H
#define MOCK_MICROCHIP_24FC_H

#include <gmock/gmock.h>
#include "Microchip_24fc.hpp"

class MockMicrochip_24FC : public Microchip_24FC {
public:
    MOCK_METHOD3(Init, ErrorStatus(I2C_HandleTypeDef* i2cHandle, uint16_t devAddress, uint32_t size));
    MOCK_METHOD3(Write, ErrorStatus(uint32_t address, uint8_t* data, uint16_t size));
    MOCK_METHOD3(Read, ErrorStatus(uint32_t address, uint8_t* data, uint16_t size));
};

#endif // MOCK_MICROCHIP_24FC_H