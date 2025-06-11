// dispatch.c
#include "dispatch.h"
#include "memory.h"

void (*dispatch_table[256])(JitContext*);

void init_dispatch_table() {
    dispatch_table[0x01] = (void(*)(JitContext*))jit_alloc_page;
    dispatch_table[0x02] = (void(*)(JitContext*))jit_free_page;
}

void jit_dispatch(uint8_t opcode, JitContext* ctx) {
    if (dispatch_table[opcode]) {
        dispatch_table[opcode](ctx);
    }
}