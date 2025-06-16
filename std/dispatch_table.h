#pragma once

#include <stdint.h>

typedef void (*DynamicHandler)(void* context);

typedef struct {
    DynamicHandler handler;
    void* context;
    uint8_t is_mutable;
} DispatchEntry;

#define MAX_OPCODES 256

// 初始化整个 dispatch 表（绑定默认 noop）
void init_dispatch_table();

// 注册一个 handler（需标注是否支持动态修改）
void register_handler(uint8_t opcode, DynamicHandler handler, void* context, uint8_t is_mutable);

// 运行时调度 handler
void trigger_handler(uint8_t opcode);

// 动态更新 handler 或 context（前提是 is_mutable）
void update_handler(uint8_t opcode, DynamicHandler new_handler);
void update_handler_context(uint8_t opcode, void* new_context);
