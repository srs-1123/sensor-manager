cmake_minimum_required(VERSION 3.10)  # 必要な最低バージョン
project(SensorManager)               # プロジェクト名

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

# libgpiodとlinux/i2c.hを使用するための設定
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)

# libgpiodのインクルードディレクトリとライブラリを設定
target_include_directories(sensor-manager PRIVATE ${LIBGPIOD_INCLUDE_DIRS})
target_link_libraries(sensor-manager PRIVATE ${LIBGPIOD_LIBRARIES})

# linux/i2c.hを使用するために必要なi2c-devライブラリをリンク
target_link_libraries(sensor-manager PRIVATE i2c)

# コンパイルオプションを追加
target_compile_options(sensor-manager PRIVATE ${LIBGPIOD_CFLAGS_OTHER})

# 実行ファイルの出力ディレクトリを設定（例: プロジェクトルートに生成）
set_target_properties(sensor-manager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
