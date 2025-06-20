#pragma once

#include <stdint.h>

typedef void (*DynamicHandler)(void* context);

typedef struct {
    DynamicHandler handler;
    void* context;
    uint8_t is_mutable;
} DispatchEntry;

#define MAX_OPCODES 256

void init_dispatch_table();
void register_handler(uint8_t opcode, DynamicHandler handler, void* context, uint8_t is_mutable);
void trigger_handler(uint8_t opcode);
void update_handler(uint8_t opcode, DynamicHandler new_handler);
void update_handler_context(uint8_t opcode, void* new_context);
