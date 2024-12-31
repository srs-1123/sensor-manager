#!/bin/bash

# ビルドディレクトリのパス（必要に応じて変更）
BUILD_DIR="build"

# 引数がない場合
if [ $# -eq 0 ]; then
    rm -rf $BUILD_DIR/*

    echo "Running 'cmake -B $BUILD_DIR -S .'..."
    cmake -B $BUILD_DIR -S .

    echo "Running 'make -C $BUILD_DIR/'..."
    make -C $BUILD_DIR
    exit 0
fi

# 引数が "clean" の場合
if [ "$1" == "clean" ]; then
    rm -rf $BUILD_DIR/*
    exit 0
fi

# 上記以外の引数の場合
echo "Error: Unsupported argument '$1'."
echo "Usage:"
echo "  No argument       -> Run 'make -C $BUILD_DIR/'"
echo "  'clean'          -> Run 'make clean -C $BUILD_DIR/'"
exit 1