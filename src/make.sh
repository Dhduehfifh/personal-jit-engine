gcc -O2 -Wall -c main.c
echo "========1========"
gcc -O2 -Wall -c builtin_func.c
echo "========2========"
gcc -O2 -Wall -c dispatch_table.c
echo "========3========"
nasm -f elf64 builtin_func.asm -o builtin_func_asm.o
echo "========4========"
nasm -f elf64 dispatch.asm -o dispatch_asm.o
echo "========5========"
gcc main.o builtin_func.o dispatch_table.o builtin_func_asm.o dispatch_asm.o -o jit_dispatch_test
echo "========DONE========"

