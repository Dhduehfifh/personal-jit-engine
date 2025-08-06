#!/bin/bash
set -e

rm -rf build

OUT_DIR=build
mkdir -p "$OUT_DIR"

echo "[1/6] 编译 bulitin_func.asm ..."
nasm -f elf64 bulitin_func.asm -o $OUT_DIR/bulitin_func.o

echo "[2/6] 编译 bulitin_func.c ..."
gcc -O0 -Wall -fPIC -c bulitin_func.c -o $OUT_DIR/bulitin_func_c.o

echo "[3/6] 编译 dispatch.c ..."
gcc -O0 -Wall -fPIC -c dispatch.c -o $OUT_DIR/dispatch.o

echo "[4/6] 编译 returns.c ..."
gcc -O0 -Wall -fPIC -c returns.c -o $OUT_DIR/returns.o

echo "[5/6] 编译 BIOS / Terminal / C API ..."
gcc -O0 -Wall -fPIC -c jmj_bios.c -o $OUT_DIR/jmj_bios.o
gcc -O0 -Wall -fPIC -c jit_c_api.c -o $OUT_DIR/jit_c_api.o
gcc -O0 -Wall -fPIC -c jit_terminal.c -o $OUT_DIR/jit_terminal.o

echo "[6/6] 链接生成 libjmj.so ..."
gcc -shared \
    $OUT_DIR/bulitin_func.o \
    $OUT_DIR/bulitin_func_c.o \
    $OUT_DIR/dispatch.o \
    $OUT_DIR/jmj_bios.o \
    $OUT_DIR/jit_c_api.o \
    $OUT_DIR/jit_terminal.o \
    $OUT_DIR/returns.o \
    -ldl -lpthread \
    -o $OUT_DIR/libjmj.so

echo "完成 ✅"
echo "运行: python3 test.py"
