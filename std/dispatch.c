#include "dispatch.h"
#include "memory.h"
#include <stdio.h>
#include "atomic_operation.h"

extern void atomic_lock_wrapper(void);
extern void atomic_unlock_wrapper(void);
extern void atomic_pause_wrapper(void);

dispatch_fn_t dispatch_table[256];

void init_dispatch_table() {
    dispatch_table[0x10] = atomic_lock_wrapper;
    dispatch_table[0x11] = atomic_unlock_wrapper;
    dispatch_table[0x12] = atomic_pause_wrapper;
}

void jit_alloc_page_handler(void* ctx) {
    JitContext* context = (JitContext*)ctx;
    void* result = jit_alloc_page(context);
    printf("[JIT] alloc_page result: %p\n", result);
}

void jit_free_page_handler(void* ctx) {
    JitContext* context = (JitContext*)ctx;
    jit_free(context);
    printf("[JIT] free_page done\n");
}

void jit_program(JitContext* ctx) {
    if (!ctx) {
        fprintf(stderr, "Error: NULL context\n");
        return;
    }

    if (!ctx->code_page && !jit_alloc_page(ctx)) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    printf("Memory allocated at: %p\n", ctx->code_page);
}
