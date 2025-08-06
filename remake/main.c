#include <stdio.h>
#include <stdlib.h>
#include "dispatch.h"

int main(void) {
    dispatch_init();
    dispatch_register_all_builtin();

    printf("CONST_NUMBER = %llu\n", (unsigned long long)CONST_NUMBER);

    uint64_t program[16];
    size_t pc = 0;

    program[pc++] = CONST_NUMBER + 0;   // alloc opcode
    program[pc++] = 1;                  // arg1 size
    program[pc++] = 2;                  // arg2 ctx_ptr

    program[pc++] = CONST_NUMBER + 2;   // terminate opcode

    size_t program_length = pc;

    printf("program content:\n");
    for (size_t i=0; i<program_length; i++) {
        printf("  program[%zu] = %llu\n", i, (unsigned long long)program[i]);
    }

    printf("开始执行测试程序...\n");

    dispatch_execute(program, 0, program_length);

    printf("执行结束。\n");

    return 0;
}



/*
gcc -Wall -O2 -o test_dispatch main.c dispatch.c bulitin_func.c returns.c -lpthread -ldl -lcrypto

*/