#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t* buffer;
    size_t capacity;
    volatile int lock;
} PageBuffer;

// 汇编函数
void asm_buffer_write(PageBuffer* page, const uint8_t* data, size_t len);
void asm_buffer_clear(PageBuffer* page);

// C接口
void init_page_buffer(PageBuffer* page, size_t capacity);
void safe_write(PageBuffer* page, const uint8_t* data, size_t len);
void destroy_page_buffer(PageBuffer* page);