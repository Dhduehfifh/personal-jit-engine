// memory.h
#pragma once
#include <stddef.h>

typedef struct {
    void* code_page;
    size_t code_offset;
    size_t code_page_size;        // ✅ 新增：实际分配大小
    size_t requested_alloc_size;  // ✅ 新增：用户希望分配的大小
} JitContext;

// 内存管理接口
void jit_init(JitContext* ctx);
void* jit_alloc_page(JitContext* ctx);
void jit_free(JitContext* ctx);  // 统一使用 jit_free