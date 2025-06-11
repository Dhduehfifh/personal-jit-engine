#pragma once
#include <stdint.h>

typedef void (*DynamicHandler)(void* context);

typedef struct {
    DynamicHandler handler;
    void* context;
    uint8_t is_mutable;
} DispatchEntry;

// 基本函数
void register_handler(uint8_t opcode, DynamicHandler handler, 
                     void* context, uint8_t is_mutable);
void trigger_handler(uint8_t opcode);

// 新增动态更新功能
void update_handler(uint8_t opcode, DynamicHandler new_handler);
void update_handler_context(uint8_t opcode, void* new_context);  // 新增函数声明