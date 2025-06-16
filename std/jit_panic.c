// jit_panic.c
#include "jit_panic.h"
#include <stdio.h>
#include <stdlib.h>

// 将错误消息数组移到文件作用域
static const char* const error_messages[] = {
    [0xDEAD0001 - 0xDEAD0001] = "Memory init failed",
    [0xDEAD0002 - 0xDEAD0001] = "Out of memory",
    [0xDEAD0003 - 0xDEAD0001] = "Invalid pointer"
};

void jit_panic(uint32_t code) {
    const char* msg;
    switch(code) {
        case 0xDEAD0001: msg = "内存初始化失败"; break;
        case 0xDEAD0002: msg = "内存耗尽"; break;
        case 0xDEAD0003: msg = "非法指针 (必须为页对齐地址)"; break;
        default: msg = "未知错误";
    }
    fprintf(stderr, "❌ JIT引擎崩溃: %s (代码:0x%X)\n", msg, code);
    fprintf(stderr, "  建议检查: 1.指针是否越界 2.内存是否已初始化\n");
    abort();
}