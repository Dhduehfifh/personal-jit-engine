#!/bin/bash

set -e  # 出错立即退出

# 清理旧文件
rm -f *.o *.so test

echo "========1========"
gcc -O2 -Wall -fPIC -c builtin_func.c -lm
echo "========2========"
gcc -O2 -Wall -fPIC -c dispatch_table.c
echo "========3========"
gcc -O2 -Wall -fPIC -c jmj_engine.c
echo "========4========"
nasm -f elf64 builtin_func.asm -o builtin_func_asm.o
echo "========5========"
nasm -f elf64 dispatch.asm -o dispatch_asm.o
echo "========6========"

# 编译为共享库 libjmj.so
gcc -shared -o libjmj.so \
    builtin_func.o \
    dispatch_table.o \
    jmj_engine.o \
    builtin_func_asm.o \
    dispatch_asm.o \
    -lm

echo "======== DONE: libjmj.so generated ========"
