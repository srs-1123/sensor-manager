#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#ifndef VCNL4040_H
#define VCNL4040_H

#include <gpiod.h>
#include <cstdint>

#define I2C_SDA_PIN 2  // GPIO3 (I2C SDA)
#define I2C_SCL_PIN 3  // GPIO2 (I2C SCL)
#define VCNL4040_I2C_ADDRESS 0x60
#define VCNL4040_AMBIENT_LIGHT_REG 0x09
#define VCNL4040_AMBIENT_CONF_REG 0x02

/**
 * @brief VCNL4040センサー専用クラス
 * GPIOを使用してI2C通信を実現し、照度データを取得します。
 */
class VCNL4040 {
private:
    struct gpiod_chip *chip;
    struct gpiod_line *scl, *sda;

    void delay();
    void set_line(struct gpiod_line *line, int value);
    int read_line(struct gpiod_line *line);
    void i2c_start();
    void i2c_stop();
    void i2c_write_bit(int bit);
    int i2c_read_bit();
    void i2c_write_byte(uint8_t byte);
    uint8_t i2c_read_byte(bool ack);

    void writeRegister(uint8_t reg, uint16_t value);
    uint16_t readRegister(uint8_t reg);

public:
    VCNL4040(const char *chip_name);
    ~VCNL4040();
    void configureSensor();
    uint16_t readAmbientLight();
};

#endif // VCNL4040_H


#endif