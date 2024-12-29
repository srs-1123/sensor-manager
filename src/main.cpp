#include "sensor-manager.h"
#include <iostream>
#include <unistd.h>

int main() {
    VCNL4040 sensor("gpiochip0"); // 使用する GPIO チップ名

    // センサーを構成
    sensor.configureSensor();

    // 照度データを読み取る
    while (true) {
        uint16_t ambientLight = sensor.readAmbientLight();
        std::cout << "Ambient Light: " << ambientLight << " lux" << std::endl;
        usleep(500000); // 0.5 秒間隔で更新
    }

    return 0;
}
