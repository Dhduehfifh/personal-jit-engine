#!/bin/bash
set -e

mkdir -p build
clang -c receiver.c -o build/receiver.o
clang -c receiver_asm.s -o build/receiver_asm.o
clang -c main.c -o build/main.o
clang build/*.o -o run_test
echo "[✅] Build success, run with ./run_test"