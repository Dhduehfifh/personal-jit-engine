#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define NUM_CALLS 1000000

void hello() { asm volatile("" ::: "memory"); }
void world() { asm volatile("" ::: "memory"); }

typedef void (*FuncPtr)();

FuncPtr func_table[2] = { hello, world };

// 外部声明汇编函数
extern void* resolve_jump_address(void** base, uint64_t index);

void run_c_call(uint8_t opcode) {
    for (int i = 0; i < NUM_CALLS; ++i) {
        func_table[opcode]();
    }
}

void run_asm_call(uint8_t opcode) {
    for (int i = 0; i < NUM_CALLS; ++i) {
        FuncPtr f = resolve_jump_address((void**)func_table, opcode);
        f();
    }
}

uint64_t now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (uint64_t)ts.tv_sec * 1e9 + ts.tv_nsec;
}

int main() {
    printf("=== Math-Jump via .s Performance Test ===\n");

    uint8_t opcode = 1;

    uint64_t t1 = now_ns();
    run_c_call(opcode);
    uint64_t t2 = now_ns();
    printf("C    call avg: %llu ns\n", (t2 - t1) / NUM_CALLS);

    uint64_t t3 = now_ns();
    run_asm_call(opcode);
    uint64_t t4 = now_ns();
    printf("ASM  call avg: %llu ns\n", (t4 - t3) / NUM_CALLS);

    return 0;
}