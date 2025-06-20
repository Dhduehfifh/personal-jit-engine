#pragma once
#ifndef DISPATCH_H
#define DISPATCH_H

#include "memory.h"

void jit_program(JitContext* ctx);
void jit_alloc_page_handler(void* ctx);
void jit_free_page_handler(void* ctx);

#endif
