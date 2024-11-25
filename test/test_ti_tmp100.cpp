#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Ti_Tmp100.hpp"
#include "mock_stm32f4xx_hal_i2c.hpp"

using ::testing::_;
using ::testing::Return;

extern MockHAL_I2C* mockHAL_I2C;

class Tmp100Test : public ::testing::Test {
protected:
    I2C_HandleTypeDef i2cHandle;
    Ti_Tmp100 tempSensor;

    void SetUp() override {
        mockHAL_I2C = new MockHAL_I2C();
        EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x48 << 1, testing::_, 2, HAL_MAX_DELAY))
            .WillOnce(testing::Return(HAL_OK));
        tempSensor.Init(&i2cHandle, 0x48);
    }

    void TearDown() override {
        delete mockHAL_I2C;
    }
};

TEST_F(Tmp100Test, InitSuccess) {
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x48 << 1, testing::_, 2, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_OK));
    ErrorStatus status = tempSensor.Init(&i2cHandle, 0x48);
    EXPECT_EQ(status, SUCCESS);
}

TEST_F(Tmp100Test, InitFailure) {
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(nullptr, 0x48 << 1, testing::_, 2, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_ERROR));
    ErrorStatus status = tempSensor.Init(nullptr, 0x48);
    EXPECT_EQ(status, ERROR);
}

TEST_F(Tmp100Test, ReadTemperatureSuccess) {
    uint8_t buffer[2] = {0x1A, 0xC0}; // Example raw temperature data
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x48 << 1, testing::_, 1, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_OK));
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Receive(&i2cHandle, 0x48 << 1, testing::_, 2, HAL_MAX_DELAY))
        .WillOnce(testing::DoAll(testing::SetArrayArgument<2>(buffer, buffer + 2), testing::Return(HAL_OK)));
    ErrorStatus status = tempSensor.ReadTemperature();
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(tempSensor.GetTemperatureRaw(), 0x1AC);
}

TEST_F(Tmp100Test, ReadTemperatureFailure) {
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(&i2cHandle, 0x48 << 1, testing::_, 1, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_OK));
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Receive(&i2cHandle, 0x48 << 1, testing::_, 2, HAL_MAX_DELAY))
        .WillOnce(testing::Return(HAL_ERROR));
    ErrorStatus status = tempSensor.ReadTemperature();
    EXPECT_EQ(status, ERROR);
}

TEST_F(Tmp100Test, ConvertRawTempToDegC) {
    float temperature = tempSensor.ConvertRawTempToDegC(0x1AC);
    EXPECT_NEAR(temperature, 26.75, 0.01); // 0x1AC * 0.0625 = 26.75
}
