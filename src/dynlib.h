#pragma once
#include <stdint.h>

#define MAX_OPCODES 256

extern void* dispatch_table[MAX_OPCODES];

// 注册函数到指定 opcode
void register_handler(uint8_t opcode, void (*handler)(void));