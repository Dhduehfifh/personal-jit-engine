// memory.c
#include "memory.h"
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>

void jit_init(JitContext* ctx) {
    memset(ctx, 0, sizeof(JitContext));
}

void* jit_alloc_page(JitContext* ctx) {
    if (!ctx) {
        fprintf(stderr, "Error: NULL context\n");
        return NULL;
    }

    if (ctx->code_page) {
        jit_free(ctx);
    }

    size_t size = ctx->requested_alloc_size;
    if (size == 0) size = 4096;  // 默认页大小
    
    void* page = mmap(NULL, size,
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      -1, 0);
    if (page == MAP_FAILED) {
        perror("jit_alloc_page mmap failed");
        return NULL;
    }

    ctx->code_page = page;
    ctx->code_offset = 0;
    ctx->code_page_size = size;  // 保存实际大小
    return page;
}

void jit_free(JitContext* ctx) {
    if (ctx && ctx->code_page) {
        printf("[JIT] Attempting to free %zu bytes at %p\n", ctx->code_page_size, ctx->code_page); // 增加日志输出
        // ✅ 修正：使用 JitContext 中保存的实际分配大小来释放内存
        if (munmap(ctx->code_page, ctx->code_page_size) == -1) {
            perror("jit_free munmap failed");
        } else {
            printf("[JIT] free_page done\n");
        }
        ctx->code_page = NULL;
        ctx->code_offset = 0;
        ctx->code_page_size = 0;        // 释放后清零
        ctx->requested_alloc_size = 0;  // 释放后清零
    } else {
        printf("[JIT] No page to free or context is NULL.\n");
    }
}