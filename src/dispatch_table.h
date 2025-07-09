#pragma once
#include <stdint.h>

// 最大opcode数量
#define MAX_DISPATCH_OPCODES 256

typedef void* (*DispatchHandler)(void* ctx);

// 全局dispatch表
extern DispatchHandler dispatch_table[MAX_DISPATCH_OPCODES];

// 注册handler
void register_handler(uint8_t opcode, DispatchHandler handler);

// 初始化内建函数注册
void init_builtin_dispatch();

// C版本调用接口（备用）
void dispatch_entry(uint8_t opcode, void* ctx);

// 汇编版本调用接口（建议用汇编）
void dispatch_entry_asm(uint8_t opcode, void* ctx);
