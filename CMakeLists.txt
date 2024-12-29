cmake_minimum_required(VERSION 3.10)  # 必要な最低バージョン
project(SensorManager)        # プロジェクト名

# C++の標準を設定（必要に応じて変更可能）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ソースコードを指定
add_executable(sensor-manager
    src/main.cpp 
    src/sensor-manager.cpp
)

# 必要なら追加のインクルードディレクトリを設定
target_include_directories(sensor-manager PRIVATE src/)

# gpiodのインクルードディレクトリを設定
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIO REQUIRED libgpiod)

# gpiodライブラリをリンク
target_link_libraries(sensor-manager PRIVATE ${LIBGPIO_LIBRARIES})

# gpiodのインクルードパスを追加
target_include_directories(sensor-manager PRIVATE ${LIBGPIO_INCLUDE_DIRS})

