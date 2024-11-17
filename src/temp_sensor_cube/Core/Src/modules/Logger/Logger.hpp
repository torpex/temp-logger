#ifndef TEMPLOGGER_HPP
#define TEMPLOGGER_HPP

#include "stm32f4xx_hal.h"
#include "Microchip_24fc.hpp"

#define LOG_ENTRY_NUM_BYTES (2)

class Logger {
public:
    Logger();
    ErrorStatus Init(Microchip_24FC* eeprom);
    ErrorStatus EraseAllLogs();
    ErrorStatus WriteLogEntry(int16_t rawTemperature);
    ErrorStatus ReadLogEntry(uint32_t* address, int16_t* result);

private:
    ErrorStatus findNextFreeAddress(uint32_t* size);
    Microchip_24FC* _eeprom;
    uint32_t _currentAddress;
};

#endif // TEMPLOGGER_HPP