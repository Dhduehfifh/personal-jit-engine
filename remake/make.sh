#!/bin/bash

set -e  # 遇到错误直接退出
set -u  # 使用未定义变量时报错

# ========== 配置 ==========
ASM_SRC="bulitin_func.asm"
C_SRCS=("bulitin_func.c" "dispatch.c" "main.c")
OUT_EXE="test"

# ========== 清理 ==========
echo "[*] 清理旧文件..."
rm -f *.o "$OUT_EXE"

# ========== 编译 NASM 汇编 ==========
echo "[*] 编译 NASM 汇编: $ASM_SRC"
nasm -f elf64 "$ASM_SRC" -o bulitin_func_asm.o

# ========== 编译 C 源文件 ==========
for src in "${C_SRCS[@]}"; do
    echo "[*] 编译 C 源文件: $src"
    gcc -O2 -Wall -fPIC -c "$src" -o "${src%.c}.o"
done

# ========== 链接 ==========
echo "[*] 链接目标文件..."
gcc -no-pie bulitin_func_asm.o bulitin_func.o dispatch.o main.o -o "$OUT_EXE" -pthread

echo "[+] 编译完成，可执行文件: $OUT_EXE"
