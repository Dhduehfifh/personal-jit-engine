#!/bin/bash
set -e



# 编译 studio 动态加载逻辑
gcc -O2 -Wall -c studio.c -o studio.o

# 编译 main 程序
gcc -O2 -Wall -o test main.c studio.o -ldl

echo "[DONE] 编译完成，运行 ./test"
