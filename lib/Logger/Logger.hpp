#ifndef TEMPLOGGER_HPP
#define TEMPLOGGER_HPP

#include "stm32f4xx_hal.h"
#include "Microchip_24fc.hpp"

#define LOG_ENTRY_NUM_BYTES (2)
#define LOG_BUFFER_MAX_ELEMENTS (EEPROM_BYTES_PER_PAGE / LOG_ENTRY_NUM_BYTES)

// Log header struct stored at the beginning of the EEPROM
typedef struct {
    uint32_t writeOffset;
} LogHeader;

// This class logs temperature data to an EEPROM
// The EEPROM is assumed to be a Microchip 24FCxxx
// The temperature data is assumed to be from a TI TMP100
// The temperature data is logged in raw counts (12-bit resolution)

class Logger {
public:
    Logger();
    ErrorStatus Init(Microchip_24FC* eeprom);
    ErrorStatus EraseAllLogs();
    ErrorStatus AddTemperatureReading(int16_t rawTemperature);
    ErrorStatus GetTemperatureReading(uint32_t* address, int16_t* result);
    ErrorStatus FlushBufferToEeprom();

private:
    LogHeader _header;
    Microchip_24FC* _eeprom;
    uint16_t _rawTempBuffer[EEPROM_BYTES_PER_PAGE];
    uint16_t _rawTempBufferIndex;

    ErrorStatus writeBufferToEeprom();
    ErrorStatus readLogHeader();
    ErrorStatus writeLogHeader();
};

#endif // TEMPLOGGER_HPP