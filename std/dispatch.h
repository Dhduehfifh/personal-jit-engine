#pragma once
#ifndef DISPATCH_TABLE_H
#define DISPATCH_TABLE_H

#include "memory.h"

void jit_program(JitContext* ctx);

// 导出 handler 给外部调用
void jit_alloc_page_handler(void* ctx);
void jit_free_page_handler(void* ctx);

typedef void (*dispatch_fn_t)(void);
extern dispatch_fn_t dispatch_table[256];

#endif
