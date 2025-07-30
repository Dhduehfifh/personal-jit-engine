// core_lib.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// 🔸 暴露接口
#define EXPORTED __attribute__((visibility("default")))

// 内存分配 / 释放
EXPORTED void* alloc_mem(size_t size) {
    return malloc(size);
}

EXPORTED void free_mem(void* ptr) {
    free(ptr);
}

// 任意数据压入内存
EXPORTED void write_data(void* base, size_t offset, const void* data, size_t size) {
    memcpy((uint8_t*)base + offset, data, size);
}

// 遍历并打印内存
EXPORTED void dump_mem(void* base, size_t size) {
    uint8_t* p = (uint8_t*)base;
    for (size_t i = 0; i < size; ++i) {
        printf("%02X ", p[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

// 普通栈结构
typedef struct {
    uint8_t* data;
    size_t size;
    size_t top;
} Stack;

EXPORTED Stack* stack_create(size_t size) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->data = (uint8_t*)malloc(size);
    s->size = size;
    s->top = 0;
    return s;
}

EXPORTED void stack_push(Stack* s, uint8_t val) {
    if (s->top < s->size) s->data[s->top++] = val;
}

EXPORTED uint8_t stack_pop(Stack* s) {
    return s->top > 0 ? s->data[--s->top] : 0;
}

EXPORTED void stack_free(Stack* s) {
    free(s->data);
    free(s);
}

// 简单输入输出
EXPORTED void write_output(const char* msg) {
    printf("OUT: %s\n", msg);
}

EXPORTED void read_input(char* buffer, size_t maxlen) {
    fgets(buffer, maxlen, stdin);
}
