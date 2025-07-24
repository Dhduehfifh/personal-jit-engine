#!/bin/bash

rm -rf *.o

# 汇编文件编译，加上 -fPIC 生成位置无关代码
nasm -f elf64 -DPIC builtin_func.asm -o builtin_func.o
nasm -f elf64 -DPIC dispatch.asm -o dispatch.o
#nasm -f elf64 -DPIC studio.asm -o studio_asm.o

# C文件编译，加上 -fPIC 和警告
gcc -c -fPIC -Wall -Wextra builtin_func.c -o builtin_func_c.o
gcc -c -fPIC -Wall -Wextra dispatch_table.c -o dispatch_table.o
gcc -c -fPIC -Wall -Wextra jmj_engine.c -o jmj_engine.o
gcc -c -fPIC -Wall -Wextra studio.c -o studio_c.o

# 链接成共享库
gcc -shared -o libjmj.so \
    builtin_func.o builtin_func_c.o \
    dispatch.o dispatch_table.o \
     studio_c.o \
    jmj_engine.o
