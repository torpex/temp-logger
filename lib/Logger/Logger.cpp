#include "Logger.hpp"
#include "Ti_Tmp100.hpp"
#include <cstring>

// Public methods

Logger::Logger() {
    _eeprom = nullptr;
    _currentAddress = 0;
}

ErrorStatus Logger::Init(Microchip_24FC* eeprom) {
    if (NULL == eeprom) {
        return ERROR;
    }

    _eeprom = eeprom;
    ErrorStatus status = findNextFreeAddress(&_currentAddress);
    if (status != SUCCESS) {
        return ERROR;
    }
    return SUCCESS;
}

ErrorStatus Logger::EraseAllLogs() {
    uint8_t emptyData[EEPROM_BYTES_PER_PAGE] = {0xFF};
    ErrorStatus status;

    for (uint32_t address = 0; address < _eeprom->GetSize(); address += EEPROM_BYTES_PER_PAGE) {
        status = _eeprom->Write(address, emptyData, EEPROM_BYTES_PER_PAGE);
        if (status != SUCCESS) {
            return ERROR;
        }
    }

    _currentAddress = 0;
    return SUCCESS;
}

ErrorStatus Logger::WriteLogEntry(int16_t rawTemperature) {
    uint8_t data[LOG_ENTRY_NUM_BYTES];
    memcpy(data, &rawTemperature, sizeof(int16_t));
    ErrorStatus status = _eeprom->Write(_currentAddress, data, sizeof(data));

    if (status != SUCCESS) {
        return ERROR;
    }

    _currentAddress += sizeof(data);
    return SUCCESS;
}

ErrorStatus Logger::ReadLogEntry(uint32_t* address, int16_t* result) {
    uint8_t data[LOG_ENTRY_NUM_BYTES];

    if (NULL == address || NULL == result) {
        return ERROR;
    }

    ErrorStatus status = _eeprom->Read(*address, data, sizeof(data));
    if (status != SUCCESS) {
        return ERROR;
    }

    memcpy(result, data, sizeof(float));
    *address += sizeof(data);
    return SUCCESS;
}

// Private methods

ErrorStatus Logger::findNextFreeAddress(uint32_t* address) {
    uint8_t data[LOG_ENTRY_NUM_BYTES];
    *address = 0;
    ErrorStatus status;

    if (NULL == address || NULL == _eeprom) {
        return ERROR;
    }

    for (uint32_t addr = 0; addr < _eeprom->GetSize(); addr += 2) {
        status = _eeprom->Read(addr, data, sizeof(data));
        if (status != SUCCESS) {
            return ERROR;
        }

        // Check if memory is empty (filled with 0xFFs)
        if (data[0] == 0xFF && data[1] == 0xFF) {
            *address = addr;
            return SUCCESS;
        }
    }

    return ERROR;
}
