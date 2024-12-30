#pragma once
#include <cstdint>

class SensorManager {
public:
    SensorManager(const char* device);
    ~SensorManager();
    uint16_t readDeviceID();
    float readAmbientLight();
    uint16_t SetALSConfig(uint8_t cmd);

private:
    int fd;
    uint16_t readRegister(uint8_t reg_addr);
    uint16_t writeRegister(uint8_t reg_addr, uint8_t lsb, uint8_t msb);
};