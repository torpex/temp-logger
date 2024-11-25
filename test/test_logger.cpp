#include <gtest/gtest.h>
#include "Logger.hpp"
#include "mock_stm32f4xx_hal_i2c.h"
#include "Microchip_24fc.hpp"

using ::testing::_;
using ::testing::Return;

#define TEMP_SENSOR_I2C_ADDRESS (0x48)   // I2C address when A0-A2 are all tied to ground
#define EEPROM_I2C_ADDRESS      (0xA0)   // I2C address of the EEPROM
#define EEPROM_SIZE             (0x8000) // 256Kbit EEPROM

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockHAL_I2C = new MockHAL_I2C();
        #include "Ti_Tmp100.hpp"
        EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(_, _, _, _, _))
            .WillOnce(Return(HAL_OK));
        EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Receive(_, _, _, _, _))
            .WillOnce(Return(HAL_OK));
        eeprom.Init(&hi2c1, EEPROM_I2C_ADDRESS, EEPROM_SIZE);
    }

    void TearDown() override {
        delete mockHAL_I2C;
    }

    I2C_HandleTypeDef hi2c1;
    Logger logger;
    Microchip_24FC eeprom;
};

TEST_F(LoggerTest, InitTest) {
    EXPECT_EQ(logger.Init(&eeprom), SUCCESS);
}

TEST_F(LoggerTest, AddTemperatureReadingTest) {
    logger.Init(&eeprom);
    int16_t rawTemperature = 2500; // Simulated raw temperature value
    EXPECT_EQ(logger.AddTemperatureReading(rawTemperature), SUCCESS);
}

TEST_F(LoggerTest, GetTemperatureReadingTest) {
    uint32_t address = 0;
    int16_t result;
    logger.Init(&eeprom);
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(_, _, _, _, _))
        .WillOnce(Return(HAL_OK));
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Receive(_, _, _, _, _))
        .WillOnce(Return(HAL_OK));
    EXPECT_EQ(logger.GetTemperatureReading(&address, &result), SUCCESS);
}

TEST_F(LoggerTest, FlushBufferToEepromTest) {
    int16_t rawTemperature = 2500; // Simulated raw temperature value
    logger.Init(&eeprom);
    logger.AddTemperatureReading(rawTemperature); // Ensure there is something to flush
    EXPECT_CALL(*mockHAL_I2C, HAL_I2C_Master_Transmit(_, _, _, _, _))
        .Times(2)
        .WillRepeatedly(Return(HAL_OK));
    EXPECT_EQ(logger.FlushBufferToEeprom(), SUCCESS);
}
