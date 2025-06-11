// memory.h
#pragma once
#include <stdint.h>

#define PAGE_SIZE 4096

void  init_memory_system();
void* jit_alloc_page();
void  jit_free_page(void* ptr);
void  jit_mark_page(void* ptr, uint8_t tag);

enum {
    MEM_INIT_FAIL = 0xDEAD0001,
    MEM_ALLOC_FAIL,
    MEM_DOUBLE_FREE,
    MEM_INVALID_PTR
};