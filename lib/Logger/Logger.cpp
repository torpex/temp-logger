#include "Logger.hpp"
#include "Ti_Tmp100.hpp"
#include <cstring>
#include <iostream>

// Defines
#define LOG_HEADER_ADDR (0)                     // Header starts on first page
#define DATA_START_ADDR (EEPROM_BYTES_PER_PAGE) // Data starts on second page

// Constructor
Logger::Logger() {
    _eeprom = nullptr;
}

// Initialize logger
// Associates EEPROM device and reads the log header
// Inputs: EEPROM handle
// Outputs: ErrorStatus (SUCCESS or ERROR)
ErrorStatus Logger::Init(Microchip_24FC* eeprom) {
    if (NULL == eeprom) {
        return ERROR;
    }
    _eeprom = eeprom;
    ErrorStatus status = readLogHeader();
    if (status != SUCCESS) {
        return ERROR;
    }
    return SUCCESS;
}

// Erase all logs in EEPROM (including header information)
// Inputs: None
// Outputs: ErrorStatus (SUCCESS or ERROR)
ErrorStatus Logger::EraseAllLogs() {
    uint8_t emptyData[EEPROM_BYTES_PER_PAGE] = {0xFF};
    ErrorStatus status;

    _header.writeOffset = 0;
    for (uint32_t address = 0; address < _eeprom->GetSize(); address += EEPROM_BYTES_PER_PAGE) {
        status = _eeprom->Write(address, emptyData, EEPROM_BYTES_PER_PAGE);
        if (status != SUCCESS) {
            return ERROR;
        }
    }

    return SUCCESS;
}

// Adds a temperature reading to RAM buffer
// If the RAM buffer is full, write it to EEPROM
ErrorStatus Logger::AddTemperatureReading(int16_t rawTemperature) {
    if (_rawTempBufferIndex < LOG_BUFFER_MAX_ELEMENTS)
    {
        _rawTempBuffer[_rawTempBufferIndex++] = rawTemperature;
        return SUCCESS;
    }
    else
    {
        ErrorStatus status = writeBufferToEeprom();
        if (status != SUCCESS)
        {
            return ERROR;
        }

        // Clear buffer and add current measurement
        _rawTempBufferIndex = 0;
        memset(_rawTempBuffer, 0, sizeof(_rawTempBuffer));
        _rawTempBuffer[_rawTempBufferIndex++] = rawTemperature;
    }

    return SUCCESS;
}

ErrorStatus Logger::GetTemperatureReading(uint32_t* address, int16_t* result) {
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

ErrorStatus Logger::FlushBufferToEeprom()
{
    ErrorStatus status = ERROR;
    if (_rawTempBufferIndex > 0)
    {
        status = writeBufferToEeprom();
    }

    return status;
}

// Private methods

ErrorStatus Logger::writeBufferToEeprom() {
    if (NULL == _eeprom) {
        return ERROR;
    }

    // Write current buffer to EEPROM
    ErrorStatus status = _eeprom->Write(_header.writeOffset, (uint8_t*)_rawTempBuffer, _rawTempBufferIndex * sizeof(uint16_t));
    if (status != SUCCESS)
    {
        return ERROR;
    }
    _header.writeOffset += _rawTempBufferIndex * sizeof(uint16_t);

    status = writeLogHeader();
    if (status != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}

ErrorStatus Logger::readLogHeader() {
    ErrorStatus status;
    if (NULL == _eeprom) {
        return ERROR;
    }

    status = _eeprom->Read(LOG_HEADER_ADDR, (uint8_t *)&_header, sizeof(_header));
    if (status != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}

ErrorStatus Logger::writeLogHeader() {
    ErrorStatus status;

    if (NULL == _eeprom) {
        return ERROR;
    }

    status = _eeprom->Write(LOG_HEADER_ADDR, (uint8_t *)&_header, sizeof(_header));
    if (status != SUCCESS) {
        return ERROR;
    }

    return SUCCESS;
}
