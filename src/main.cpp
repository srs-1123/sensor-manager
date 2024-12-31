#include "sensor-manager.h"
#include <iostream>
#include <unistd.h>

int main() {
// #define VCNL4040
    try {
#if VCNL4040
        // VCNL4040Managerクラスを初期化
        VCNL4040Manager sensor("/dev/i2c-1"); // 使用するI2Cバスに応じて変更

        // デバイスID確認
        uint16_t device_id = sensor.readDeviceID();
        std::cout << "Device ID: 0x" << std::hex << device_id << std::endl;  // デバッグ出力
        if (device_id != 0x0186) {
            // close(fd);
            throw std::runtime_error("Invalid device ID: " + std::to_string(device_id));
        }

        sensor.SetALSConfig(0x60); // ALSの設定を変更

        // 設定変更後の待機時間（例: 100ms）
        usleep(500000); // 100,000マイクロ秒 = 100ミリ秒

        // 照度と近接データを読み取り
        while (true) {
            uint16_t ambientLight = sensor.readAmbientLight();
            // uint16_t proximity = sensor.readProximity();

            std::cout << "Ambient Light: " << std::dec << ambientLight << " lux" << std::endl;
            // std::cout << "Proximity: " << proximity << std::endl;

            sleep(1); // 1秒待機
        }
#else 
    GPIOManager sensor(17);
    while (true) {
        std::cout << "GPIO17 Value: " << sensor.getValue() << std::endl;
        usleep(500000); // 500ms待機
    }
#endif
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}