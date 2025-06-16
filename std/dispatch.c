#include "dispatch.h"
#include "memory.h"
#include <stdio.h>

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
