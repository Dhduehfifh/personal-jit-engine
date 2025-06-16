// jit_panic.h
#pragma once
#include <stdint.h>

// 错误码定义
typedef enum {
    JIT_PANIC_NONE = 0,
    JIT_PANIC_MEM_INIT_FAIL,      // 内存初始化失败
    JIT_PANIC_OUT_OF_MEMORY,      // 内存耗尽
    JIT_PANIC_INVALID_FREE,       // 非法释放
    JIT_PANIC_DOUBLE_FREE,        // 重复释放
    JIT_PANIC_ILLEGAL_OPCODE,     // 非法指令
    JIT_PANIC_STACK_OVERFLOW,     // 栈溢出
    JIT_PANIC_GC_CORRUPTION       // GC数据损坏
} JitPanicCode;

// 紧急处理函数
void jit_panic(uint32_t code);

// 设置自定义panic处理器
typedef void(*JitPanicHandler)(uint32_t code);
void set_jit_panic_handler(JitPanicHandler handler);