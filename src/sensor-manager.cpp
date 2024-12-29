#include "sensor-manager.h"
#include <iostream>
#include <unistd.h> // for usleep

VCNL4040::VCNL4040(const char *chip_name) {
    chip = gpiod_chip_open_by_name(chip_name);
    if (!chip) {
        perror("Failed to open GPIO chip");
        exit(1);
    }

    scl = gpiod_chip_get_line(chip, I2C_SCL_PIN);
    sda = gpiod_chip_get_line(chip, I2C_SDA_PIN);

    if (!scl || !sda) {
        perror("Failed to get GPIO line");
        exit(1);
    }

    if (gpiod_line_request_output(scl, "VCNL4040 SCL", 0) < 0 ||
        gpiod_line_request_output(sda, "VCNL4040 SDA", 0) < 0) {
        perror("Failed to set GPIO lines to output");
        exit(1);
    }

    if (gpiod_line_set_flags(scl, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) < 0 ||
        gpiod_line_set_flags(sda, GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP) < 0) {
        perror("Failed to set pull-up bias");
        exit(1);
    }
}

VCNL4040::~VCNL4040() {
    gpiod_line_release(scl);
    gpiod_line_release(sda);
    gpiod_chip_close(chip);
}

void VCNL4040::delay() {
    usleep(5); // I2C timing: 約5μs
}

void VCNL4040::set_line(struct gpiod_line *line, int value) {
    if (gpiod_line_set_value(line, value) < 0) {
        perror("Failed to set GPIO line value");
        exit(1);
    }
}

int VCNL4040::read_line(struct gpiod_line *line) {
    int value = gpiod_line_get_value(line);
    if (value < 0) {
        perror("Failed to read GPIO line value");
        exit(1);
    }
    return value;
}

void VCNL4040::i2c_start() {
    set_line(sda, 1);
    set_line(scl, 1);
    delay();
    set_line(sda, 0);
    delay();
    set_line(scl, 0);
}

void VCNL4040::i2c_stop() {
    set_line(sda, 0);
    set_line(scl, 1);
    delay();
    set_line(sda, 1);
    delay();
}

void VCNL4040::i2c_write_bit(int bit) {
    set_line(sda, bit);
    delay();
    set_line(scl, 1);
    delay();
    set_line(scl, 0);
}

int VCNL4040::i2c_read_bit() {
    set_line(sda, 1); // SDA を入力モードに
    delay();
    set_line(scl, 1);
    delay();
    int bit = read_line(sda);
    set_line(scl, 0);
    return bit;
}

void VCNL4040::i2c_write_byte(uint8_t byte) {
    for (int i = 7; i >= 0; --i) {
        i2c_write_bit((byte >> i) & 0x01);
    }
    i2c_read_bit(); // ACK を読み取る
}

uint8_t VCNL4040::i2c_read_byte(bool ack) {
    uint8_t byte = 0;
    for (int i = 7; i >= 0; --i) {
        byte = (byte << 1) | i2c_read_bit();
    }
    i2c_write_bit(ack ? 0 : 1); // ACK を送信
    return byte;
}

void VCNL4040::writeRegister(uint8_t reg, uint16_t value) {
    i2c_start();
    i2c_write_byte((VCNL4040_I2C_ADDRESS << 1) | 0); // 書き込みアドレス
    i2c_write_byte(reg);
    i2c_write_byte(value & 0xFF);     // LSB
    i2c_write_byte((value >> 8) & 0xFF); // MSB
    i2c_stop();
}

uint16_t VCNL4040::readRegister(uint8_t reg) {
    i2c_start();
    i2c_write_byte((VCNL4040_I2C_ADDRESS << 1) | 0); // 書き込みアドレス
    i2c_write_byte(reg);
    i2c_start();
    i2c_write_byte((VCNL4040_I2C_ADDRESS << 1) | 1); // 読み取りアドレス
    uint8_t lowByte = i2c_read_byte(true);
    uint8_t highByte = i2c_read_byte(false);
    i2c_stop();
    return (highByte << 8) | lowByte;
}

void VCNL4040::configureSensor() {
    // 照度測定を有効化する設定を送信
    writeRegister(VCNL4040_AMBIENT_CONF_REG, 0x1000);
    usleep(100000); // 設定が反映されるまで待機
}

uint16_t VCNL4040::readAmbientLight() {
    return readRegister(VCNL4040_AMBIENT_LIGHT_REG);
}
