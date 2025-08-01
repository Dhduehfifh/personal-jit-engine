#!/bin/bash
set -e

rm -f *.o test
nasm -f elf64 plugin_call_bridge.asm -o plugin_call_bridge.o
gcc -c -fPIC main.c -o main.o
gcc -no-pie main.o plugin_call_bridge.o -ldl -o test -pthread
