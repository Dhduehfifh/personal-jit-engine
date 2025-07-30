#!/bin/bash

# 清理旧文件
rm -rf *.o
rm -f test

# 编译 NASM 汇编文件，支持 PIE（加 -DPIC）
nasm -f elf64 -DPIC bulitin_func.asm -o bulitin_func_asm.o

# 编译 C 文件，支持 Position-Independent Code
gcc -c -fPIC bulitin_func.c -o bulitin_func_c.o
gcc -c -fPIC main.c -o main.o

# 链接，生成 PIE 可执行文件
gcc -pie bulitin_func_asm.o bulitin_func_c.o main.o -o test
