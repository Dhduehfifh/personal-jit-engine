// jit_core.c
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "jit_core.h"

// 函数表
jit_func_t dispatch_table[PAGE_MAX][OPCODE_MAX] = {0};
jit_func_t cold_func_pool[PAGE_MAX][OPCODE_MAX] = {0};
uint8_t* hot_exec_area = NULL;

__attribute__((constructor))
void init_exec_area() {
    hot_exec_area = mmap(NULL, HOT_EXEC_SIZE,
                         PROT_READ | PROT_WRITE | PROT_EXEC,
                         MAP_ANON | MAP_PRIVATE, -1, 0);
    if (hot_exec_area == MAP_FAILED) {
        perror("mmap exec area failed");
    }
}

// 热加载函数：将冷区函数复制到热区并绑定到 dispatch_table
void load_func_to_hot(uint8_t page, uint8_t opcode) {
    if (page >= PAGE_MAX || opcode >= OPCODE_MAX) return;

    jit_func_t target = cold_func_pool[page][opcode];
    if (!target) {
        printf("ERROR: No cold function at page %d, opcode %d\n", page, opcode);
        return;
    }

    // 拷贝函数体（假设最多256字节）
    memcpy(hot_exec_area, (void*)target, 256);
    dispatch_table[page][opcode] = (jit_func_t)hot_exec_area;
    printf("[loader] Loaded func (%d, %d) to hot area\n", page, opcode);
}

// 示例函数 1
void func_add() {
    int a = 2, b = 3;
    printf("[exec] ADD result: %d\n", a + b);
}

// 示例函数 2
void func_print() {
    printf("[exec] Hello from func_print()\n");
}
