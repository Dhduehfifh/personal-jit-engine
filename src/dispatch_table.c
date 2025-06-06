#include "dispatch_table.h"
#include <stdio.h>

void* dispatch_table[MAX_OPCODES] = {0};

// 示例事件实体函数
void handler_malloc() {
    printf("[EVENT] malloc() called\n");
}
void handler_write() {
    printf("[EVENT] write() called\n");
}
void handler_custom() {
    printf("[EVENT] custom event triggered\n");
}

// 初始化
__attribute__((constructor))
void init_dispatch_table() {
    register_handler(0x01, handler_malloc);
    register_handler(0x02, handler_write);
    register_handler(0x03, handler_custom);
}

void register_handler(uint8_t opcode, void (*handler)(void)) {
    dispatch_table[opcode] = handler;
}