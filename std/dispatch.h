#pragma once

#include "memory.h"

void jit_program(JitContext* ctx);

// 导出 handler 给外部调用
void jit_alloc_page_handler(void* ctx);
void jit_free_page_handler(void* ctx);
