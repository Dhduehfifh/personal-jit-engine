#!/bin/bash
set -e

OUT="jit_engine"
SRC="main.c"

echo "[🔨] Detecting compiler..."
if command -v clang > /dev/null; then
    CC=clang
elif command -v gcc > /dev/null; then
    CC=gcc
else
    echo "❌ No suitable compiler found (gcc/clang)."
    exit 1
fi

mkdir -p bin
echo "[🚀] Compiling with $CC..."
$CC -std=c11 -O2 $SRC -o bin/$OUT

echo "[✅] Build complete -> bin/$OUT"