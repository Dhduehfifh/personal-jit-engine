// main.c
#include <stdio.h>
#include "jit_core.h"

// 汇编声明
extern void dispatch_call(uint8_t page, uint8_t opcode);

int main() {
    // 注册冷函数
    cold_func_pool[0][1] = func_add;
    cold_func_pool[0][2] = func_print;

    // 第一次使用，热加载
    load_func_to_hot(0, 1);
    dispatch_call(0, 1);  // 调用 func_add

    load_func_to_hot(0, 2);
    dispatch_call(0, 2);  // 调用 func_print

    // 重复调用，无需再加载
    dispatch_call(0, 1);
    return 0;
}
