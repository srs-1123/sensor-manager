#pragma once
#include <cstdint>

#include <gpiod.h>

class VCNL4040Manager {
public:
    VCNL4040Manager(const char* device);
    ~VCNL4040Manager();
    uint16_t readDeviceID();
    float readAmbientLight();
    uint16_t SetALSConfig(uint8_t cmd);

private:
    int fd;
    uint16_t readRegister(uint8_t reg_addr);
    uint16_t writeRegister(uint8_t reg_addr, uint8_t lsb, uint8_t msb);
};

class GPIOManager {
public:
    GPIOManager(unsigned int line_num);
    ~GPIOManager();
    uint16_t getValue();
private:
    gpiod_chip* chip;
    gpiod_line* line;
};