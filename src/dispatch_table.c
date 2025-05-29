#include "dispatch_table.h"
#include <stdio.h>

void* dispatch_table[MAX_OPCODES] = {0};

void register_handler(uint8_t opcode, void (*handler)(void)) {
    dispatch_table[opcode] = handler;
}

void malloc_event() {
    printf("[EVENT] malloc() called\n");
}

void write_event() {
    printf("[EVENT] write() called\n");
}

void custom_event() {
    printf("[EVENT] custom() called\n");
}

__attribute__((constructor))
void init_dispatch_table() {
    register_handler(0x01, malloc_event);
    register_handler(0x02, write_event);
    register_handler(0x03, custom_event);
}