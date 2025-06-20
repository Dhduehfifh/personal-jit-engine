#include "dispatch_table.h"
#include <stdio.h>
#include "atomic_operation.h"
#include "memory.h"

extern void _op_add(), _op_sub(), _op_mul(), _op_div();
extern void atomic_lock_wrapper(void);
extern void atomic_unlock_wrapper(void);
extern void atomic_pause_wrapper(void);

static DispatchEntry dispatch_table[MAX_OPCODES];

static void handler_noop(void* context) {
    (void)context;
    printf("[DISPATCH] NOOP handler triggered\n");
}

void init_dispatch_table() {
    for (int i = 0; i < MAX_OPCODES; ++i) {
        dispatch_table[i].handler = handler_noop;
        dispatch_table[i].context = NULL;
        dispatch_table[i].is_mutable = 1;
    }

    dispatch_table[0x04].handler = (DynamicHandler)_op_add;
    dispatch_table[0x05].handler = (DynamicHandler)_op_sub;
    dispatch_table[0x06].handler = (DynamicHandler)_op_mul;
    dispatch_table[0x07].handler = (DynamicHandler)_op_div;

    dispatch_table[0x10].handler = (DynamicHandler)atomic_lock_wrapper;
    dispatch_table[0x11].handler = (DynamicHandler)atomic_unlock_wrapper;
    dispatch_table[0x12].handler = (DynamicHandler)atomic_pause_wrapper;
}

void register_handler(uint8_t opcode, DynamicHandler handler, void* context, uint8_t is_mutable) {
    dispatch_table[opcode].handler = handler;
    dispatch_table[opcode].context = context;
    dispatch_table[opcode].is_mutable = is_mutable;
}

void trigger_handler(uint8_t opcode) {
    if (opcode >= MAX_OPCODES) {
        printf("[ERROR] Invalid opcode 0x%02X\n", opcode);
        return;
    }
    DispatchEntry* entry = &dispatch_table[opcode];
    entry->handler(entry->context);
}

void update_handler(uint8_t opcode, DynamicHandler new_handler) {
    if (opcode >= MAX_OPCODES || !dispatch_table[opcode].is_mutable) {
        printf("[WARN] Opcode 0x%02X is not mutable or invalid\n", opcode);
        return;
    }
    dispatch_table[opcode].handler = new_handler;
}

void update_handler_context(uint8_t opcode, void* new_context) {
    if (opcode >= MAX_OPCODES || !dispatch_table[opcode].is_mutable) {
        printf("[WARN] Cannot update context for opcode 0x%02X\n", opcode);
        return;
    }
    dispatch_table[opcode].context = new_context;
}
