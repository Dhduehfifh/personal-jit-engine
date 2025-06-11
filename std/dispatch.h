// dispatch.h
#pragma once
#include <stdint.h>

typedef struct {
    uint64_t x[29];
    uint64_t fp, lr, sp;
} JitContext;

extern void (*dispatch_table[256])(JitContext*);

void init_dispatch_table();
void jit_dispatch(uint8_t opcode, JitContext* ctx);