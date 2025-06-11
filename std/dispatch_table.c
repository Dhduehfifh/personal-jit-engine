#include "dispatch_table.h"
#include <stddef.h>

#define MAX_OPCODES 256
static DispatchEntry dispatch_table[MAX_OPCODES] = {{0}};

void register_handler(uint8_t opcode, DynamicHandler handler, 
                     void* context, uint8_t is_mutable) {
    if (opcode >= MAX_OPCODES) return;
    dispatch_table[opcode] = (DispatchEntry){
        .handler = handler,
        .context = context,
        .is_mutable = is_mutable
    };
}

void trigger_handler(uint8_t opcode) {
    if (opcode >= MAX_OPCODES || !dispatch_table[opcode].handler) return;
    dispatch_table[opcode].handler(dispatch_table[opcode].context);
}

void update_handler(uint8_t opcode, DynamicHandler new_handler) {
    if (opcode < MAX_OPCODES && dispatch_table[opcode].is_mutable) {
        dispatch_table[opcode].handler = new_handler;
    }
}

// 新增函数实现
void update_handler_context(uint8_t opcode, void* new_context) {
    if (opcode < MAX_OPCODES) {
        dispatch_table[opcode].context = new_context;
    }
}