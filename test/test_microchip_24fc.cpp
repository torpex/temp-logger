#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Microchip_24fc.hpp"
#include "mock_stm32f4xx_hal_i2c.hpp"

using ::testing::_;
using ::testing::Return;

extern MockHAL_I2C* mockHAL_I2C;

class Microchip24FCTest : public ::testing::Test {
protected:
    I2C_HandleTypeDef i2cHandle;
    Microchip_24FC eeprom;

    void SetUp() override {
        mockHAL_I2C = new MockHAL_I2C();
        eeprom.Init(&i2cHandle, 0x50, 8192); // Example address and size
    }

    void TearDown() override {
        delete mockHAL_I2C;
    }
};

TEST_F(Microchip24FCTest, InitSuccess) {
    ErrorStatus status = eeprom.Init(&i2cHandle, 0x50, 8192);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(eeprom.GetSize(), 8192);
}

TEST_F(Microchip24FCTest, WriteSuccess) {
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t buffer[6] = {0x00, 0x10, 0x01, 0x02, 0x03, 0x04}; // Address 0x0010 + data
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x50 << 1, testing::_, 6, HAL_MAX_DELAY))
        .WillOnce(testing::DoAll(testing::SetArrayArgument<2>(buffer, buffer + 6), testing::Return(HAL_OK)));
    ErrorStatus status = eeprom.Write(0x0010, data, 4);
    EXPECT_EQ(status, SUCCESS);
}

TEST_F(Microchip24FCTest, WriteFailure) {
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x50 << 1, testing::_, 6, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_ERROR));
    ErrorStatus status = eeprom.Write(0x0010, data, 4);
    EXPECT_EQ(status, ERROR);
}

TEST_F(Microchip24FCTest, ReadSuccess) {
    uint8_t data[4];
    uint8_t expectedData[4] = {0x01, 0x02, 0x03, 0x04};
    uint8_t memAddrBuffer[2] = {0x00, 0x10}; // Address 0x0010
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x50 << 1, testing::_, 2, HAL_MAX_DELAY))
        .WillOnce(testing::DoAll(testing::SetArrayArgument<2>(memAddrBuffer, memAddrBuffer + 2), testing::Return(HAL_OK)));
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Receive(&i2cHandle, 0x50 << 1, testing::_, 4, HAL_MAX_DELAY))
        .WillOnce(testing::DoAll(testing::SetArrayArgument<2>(expectedData, expectedData + 4), testing::Return(HAL_OK)));
    ErrorStatus status = eeprom.Read(0x0010, data, 4);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_THAT(data, testing::ElementsAreArray(expectedData));
}

TEST_F(Microchip24FCTest, ReadFailure) {
    uint8_t data[4];
    uint8_t memAddrBuffer[2] = {0x00, 0x10}; // Address 0x0010
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x50 << 1, testing::_, 2, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_ERROR));
    ErrorStatus status = eeprom.Read(0x0010, data, 4);
    EXPECT_EQ(status, ERROR);
}
