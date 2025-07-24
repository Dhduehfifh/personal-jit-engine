rm -rf *.o

nasm -f elf64 -DPIC builtin_func.asm -o builtin_func.o
nasm -f elf64 -DPIC dispatch.asm -o dispatch.o
nasm -f elf64 -DPIC studio.asm -o studio_asm.o   # 确保这里也加 -DPIC

gcc -c -fPIC builtin_func.c -o builtin_func_c.o
gcc -c -fPIC dispatch_table.c -o dispatch_table.o
gcc -c -fPIC jmj_engine.c -o jmj_engine.o
gcc -c -fPIC studio.c -o studio_c.o

gcc -shared -o libjmj.so \
    builtin_func.o builtin_func_c.o \
    dispatch.o dispatch_table.o \
    studio_asm.o studio_c.o \
    jmj_engine.o
