#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Logger.hpp"
#include "mock_stm32f4xx_hal_i2c.hpp"
#include "mock_microchip_24fc.hpp"

using ::testing::_;
using ::testing::Return;

#define TEMP_SENSOR_I2C_ADDRESS (0x48)   // I2C address when A0-A2 are all tied to ground
#define EEPROM_I2C_ADDRESS      (0xA0)   // I2C address of the EEPROM
#define EEPROM_SIZE             (0x8000) // 256Kbit EEPROM

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockHAL_I2C = new MockHAL_I2C();
        mockEEPROM = new MockMicrochip_24FC();
    }

    void TearDown() override {
        delete mockHAL_I2C;
        delete mockEEPROM;
    }

    I2C_HandleTypeDef hi2c1;
    Logger logger;
    MockMicrochip_24FC* mockEEPROM;
    MockHAL_I2C* mockHAL_I2C;
};

TEST_F(LoggerTest, InitTest) {
    EXPECT_CALL(*mockEEPROM, Read(_, _, _))
        .Times(1)
        .WillOnce(Return(SUCCESS));
    EXPECT_EQ(logger.Init(mockEEPROM), SUCCESS);
}

TEST_F(LoggerTest, AddTemperatureReading_MidBufferTest) {
    EXPECT_CALL(*mockEEPROM, Read(_, _, _))
        .Times(1)
        .WillOnce(Return(SUCCESS));
    logger.Init(mockEEPROM);

    int16_t rawTemperature = 2500; // Simulated raw temperature value
    EXPECT_EQ(logger.AddTemperatureReading(rawTemperature), SUCCESS);
}

TEST_F(LoggerTest, AddTemperatureReading_EndOfBufferTest) {
    EXPECT_CALL(*mockEEPROM, Read(_, _, _))
        .Times(1)
        .WillOnce(Return(SUCCESS));
    logger.Init(mockEEPROM);

    EXPECT_CALL(*mockEEPROM, Write(_, _, _))
        .Times(2)
        .WillRepeatedly(Return(SUCCESS));
    int16_t rawTemperature = 2500; // Simulated raw temperature value
    for (uint8_t i = 0; i < LOG_BUFFER_MAX_ELEMENTS+1; i++)
    {
        EXPECT_EQ(logger.AddTemperatureReading(rawTemperature), SUCCESS);
    }
}

TEST_F(LoggerTest, GetTemperatureReadingTest) {
    EXPECT_CALL(*mockEEPROM, Read(_, _, _))
        .Times(1)
        .WillOnce(Return(SUCCESS));
    logger.Init(mockEEPROM);

    EXPECT_CALL(*mockEEPROM, Read(_, _, _))
        .Times(1)
        .WillOnce(Return(SUCCESS));
    uint32_t address = 0;
    int16_t result;
    EXPECT_EQ(logger.GetTemperatureReading(&address, &result), SUCCESS);
}

TEST_F(LoggerTest, FlushBufferToEepromTest) {
    EXPECT_CALL(*mockEEPROM, Read(_, _, _))
        .Times(1)
        .WillOnce(Return(SUCCESS));
    logger.Init(mockEEPROM);

    EXPECT_CALL(*mockEEPROM, Write(_, _, _))
        .Times(2)
        .WillRepeatedly(Return(SUCCESS));
    int16_t rawTemperature = 2500; // Simulated raw temperature value
    logger.AddTemperatureReading(rawTemperature); // Ensure there is something to flush
    EXPECT_EQ(logger.FlushBufferToEeprom(), SUCCESS);
}
