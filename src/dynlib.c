#include "dynlib.h"
#include <stdio.h>
#include <string.h>

void* dispatch_table[MAX_OPCODES] = {0};

void register_handler(uint8_t opcode, void (*handler)(void)) {
    dispatch_table[opcode] = handler;
}

// === 示例事件实体函数 ===
void malloc_event() {
    printf("[EVENT] malloc() called\n");
}

void write_event() {
    printf("[EVENT] write() called\n");
}

void custom_event() {
    printf("[EVENT] custom() called\n");
}

// === 自动注册 ===
__attribute__((constructor))
void init_dispatch_table() {
    register_handler(0x01, malloc_event);
    register_handler(0x02, write_event);
    register_handler(0x03, custom_event);
}