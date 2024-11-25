# Temp-Logger

## Description

A simple temperature logger application which reads data from a TMP100 sensor every 10 minutes and periodically saves the data to a 24FC series EEPROM (once every ~5.3 hours by default).

### Assumptions
  - An STM32F446 microcontroller is used as the main micro.
  - The TMP100 and 24FC share the same physical I2C bus.
  - The TMP100 address lines ADD0 and ADD1 are tied to ground, so the I2C address is 0b01001000 (0x48).
  - 24FC address lines A0, A1, and A2 are tied to ground, so the I2C address is 0b10100000 (0xA0).
  - 24FC write protect line (WP) is tied to ground enable writing.
  - Appropriate pull up is installed on the SDA line (2k-10k ohm).

### Notes
   - To minimize EEPROM wear, temp readings are buffered in RAM and then written to the EEPROM
     only once a full 64 byte page buffer has been populated, unless the user calls FlushBufferToEeprom() to expedite the write.
     With a 64 byte page using 2 bytes per sample collected every 10 mins, this default behavior corresponds to about 5.33 hours of buffering between EEPROM writes.

   - The current implementation should allow for at least ~10 years of continous logging before the EEPROM write limits are reached (limited by the first page with the header/write offset which gets updated every time the 64 byte RAM buffer is completely filled and therefore flushed to the EEPROM).

   - The EEPROM life can be extended by increasing the RAM buffer size or by implementing some form of wear leveling on the EEPROM header.  Increasing the RAM buffer too much will expose more data to loss in the event of a power cycle, so a balance must be struck between EEPROM wear and data loss risk.

   - The first page in the EEPROM is allocated to the header information which stores the write offset so that the logger can resume writing to the proper place in the event of a power interruption.

   - Temperature is logged in raw counts (12-bit resolution) as a uint16_t to save space (vs a 32 bit float).  However, ConvertRawTempToDegC() is available to convert the raw temp to degrees C.

   - Power cycles will cause logging to resume at the next available address in EEPROM (RTC would be needed to track time).

   - The code uses a simple HAL delay here to keep code simple, but generally one would use an RTOS or timer interrupt in a real application in which the code was doing more than just reading/logging temperature data.

   - The code and unit tests should both compile and run successfully under PlatformIO which can be installed as a VScode plugin.

### Regular code layout
* lib/Logger - Temperature logging module
* lib/Microchip_24fc - EEPROM driver
* lib/Ti_Tmp100 - Temperature sensor driver
* src/Src/main.cpp - Program entry point / logging loop

### Unit tests
* test/logger - Unit tests for logger
* test/microchip_24fc - Unit tests for EEPROM
* test/ti_tmp100 - Unit tests for temperature sensor

## Getting Started

### Dependencies

* PlatformIO toolchain
* VScode (optional but recommended)

### Executing program / unit tests

* VScode->Tasks->Run Task->PlatformIO (Build)
* VScode->Tasks->Run Task->PlatformIO Test (unit tests)

## Authors

Cameron Lewis [@torpex]