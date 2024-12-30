#include "sensor-manager.h"
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdexcept>
#include <iostream>
#include <cstring>

#define VCNL4040_ALS_CONF 0x00
#define VCNL4040_I2C_ADDRESS 0x60
#define VCNL4040_DEVICE_ID 0x0C
#define VCNL4040_ALS_DATA 0x09

SensorManager::SensorManager(const char *i2c_device) {
    fd = open(i2c_device, O_RDWR);
    if (fd < 0) {
        perror("Failed to open I2C device");
        throw std::runtime_error("I2C device open error");
    }
}

SensorManager::~SensorManager() {
    if (fd >= 0) {
        close(fd);
    }
}

uint16_t SensorManager::readDeviceID() {
    return readRegister(VCNL4040_DEVICE_ID);
}

float SensorManager::readAmbientLight() {
    uint16_t raw_light = readRegister(VCNL4040_ALS_DATA);
    
    // デバッグ出力
    std::cout << "Raw ALS: 0x" << std::hex << raw_light 
              << " (" << std::dec << raw_light << ")" << std::endl;
    
    // 0.01 lux/step変換
    float lux = raw_light * 0.1;
    // std::cout << "Ambient Light: " << lux << " Lux" << std::endl;
    
    return lux;
}

uint16_t SensorManager::SetALSConfig(uint8_t cmd) {
    return writeRegister(VCNL4040_ALS_CONF, cmd, 0x00);
}

uint16_t SensorManager::readRegister(uint8_t reg_addr) {
    uint8_t buffer[2];
    struct i2c_msg messages[] = {
      { VCNL4040_I2C_ADDRESS, 0, 1, &reg_addr },      /* レジスタアドレスをセット. */
      { VCNL4040_I2C_ADDRESS, I2C_M_RD, 2, buffer },  /* dataにlengthバイト読み込む. */
    };
    struct i2c_rdwr_ioctl_data ioctl_data = { messages, 2 };

    /* I2C-Readを行う. */
    if (ioctl(fd, I2C_RDWR, &ioctl_data) < 0) {
        fprintf(stderr, "i2c_read: failed to ioctl: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    // デバッグ出力
    uint16_t data = (buffer[1] << 8) | buffer[0];
    // std::cout << "Read data: 0x" << std::hex << data << std::endl;

    // LSBとMSBを16ビット値に結合
    return data;
}

uint16_t SensorManager::writeRegister(uint8_t reg_addr, uint8_t lsb, uint8_t msb) {
    /* I2C-Write用のバッファを準備する. */
    uint8_t buffer[3];
    if (buffer == NULL) {
        fprintf(stderr, "i2c_write: failed to memory allocate\n");
        close(fd);
        return -1;
    }
    buffer[0] = reg_addr;  // 1バイト目にレジスタアドレスをセット
    buffer[1] = lsb;       // 2バイト目にLSBをセット
    buffer[2] = msb;       // 3バイト目にMSBをセット

    /* I2C-Writeメッセージを作成する. */
    struct i2c_msg message = { VCNL4040_I2C_ADDRESS, 0, 3, buffer };
    struct i2c_rdwr_ioctl_data ioctl_data = { &message, 1 };

    /* I2C-Writeを行う. */
    if (ioctl(fd, I2C_RDWR, &ioctl_data) < 0) {
        fprintf(stderr, "i2c_write: failed to ioctl: %s\n", strerror(errno));
        close(fd);
        return -1;
    }
    return 0;
}