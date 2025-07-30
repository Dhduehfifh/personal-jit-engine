/*
 * jit_demo.c - 多平台 JIT/偏移栈/DSL 操作基础实现
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>
#endif

// =====================================
// 1. alloc / free
// =====================================
void* jit_alloc(size_t size) {
    void* ptr = malloc(size);
    printf("[jit_alloc] %zu bytes @ %p\n", size, ptr);
    return ptr;
}

void jit_free(void* ptr) {
    printf("[jit_free] free @ %p\n", ptr);
    free(ptr);
}

// =====================================
// 2. 写入数据到内存块
// =====================================
void jit_write(void* base, size_t offset, const void* data, size_t size) {
    memcpy((uint8_t*)base + offset, data, size);
}

// =====================================
// 3. 遍历内存块内容 (假设为 uint32_t)
// =====================================
void jit_traverse_u32(void* base, size_t count) {
    uint32_t* arr = (uint32_t*)base;
    for (size_t i = 0; i < count; ++i) {
        printf("[%zu] = %u\n", i, arr[i]);
    }
}

// =====================================
// 4. DSL环形栈 & 普通栈
// =====================================
typedef struct {
    uint8_t* buffer;
    size_t capacity;
    size_t top;
} Stack;

void stack_init(Stack* s, size_t size) {
    s->buffer = malloc(size);
    s->capacity = size;
    s->top = 0;
}

void stack_push(Stack* s, uint8_t val) {
    if (s->top < s->capacity) s->buffer[s->top++] = val;
}

uint8_t stack_pop(Stack* s) {
    return s->top > 0 ? s->buffer[--s->top] : 0;
}

void stack_free(Stack* s) {
    free(s->buffer);
}

// 环形 DSL 栈
#define RING_CAP 64

typedef struct {
    uint8_t buf[RING_CAP];
    size_t head;
    size_t tail;
} RingStack;

void ring_push(RingStack* r, uint8_t val) {
    r->buf[r->head] = val;
    r->head = (r->head + 1) % RING_CAP;
    if (r->head == r->tail) r->tail = (r->tail + 1) % RING_CAP;
}

uint8_t ring_pop(RingStack* r) {
    if (r->head == r->tail) return 0;
    r->head = (r->head - 1 + RING_CAP) % RING_CAP;
    return r->buf[r->head];
}

// =====================================
// 5. 动态加载 + 函数列出 (示例 libc 中 printf)
// =====================================
void* jit_load_lib(const char* path) {
#ifdef _WIN32
    return LoadLibraryA(path);
#else
    return dlopen(path, RTLD_LAZY);
#endif
}

void* jit_get_func(void* handle, const char* name) {
#ifdef _WIN32
    return GetProcAddress((HMODULE)handle, name);
#else
    return dlsym(handle, name);
#endif
}

// =====================================
// 6. 指针偏移暴露
// =====================================
uint8_t jit_read_u8(void* base, size_t offset) {
    return *((uint8_t*)base + offset);
}

// =====================================
// 7. 接口暴露（作为DLL/so导出）
// =====================================
#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

EXPORT void* export_alloc(size_t size) { return jit_alloc(size); }
EXPORT void export_free(void* ptr) { jit_free(ptr); }
EXPORT void export_write(void* base, size_t offset, const void* data, size_t size) { jit_write(base, offset, data, size); }
EXPORT uint8_t export_read_u8(void* base, size_t offset) { return jit_read_u8(base, offset); }

// =====================================
// 8. I/O 接口（打印）
// =====================================
EXPORT void export_output_str(const char* str) { puts(str); }
EXPORT void export_output_u32(uint32_t val) { printf("VAL: %u\n", val); }

// =====================================
// main (可选测试)
// =====================================
#ifdef TEST_MAIN
int main() {
    void* mem = jit_alloc(32);
    uint32_t val = 123456;
    jit_write(mem, 4, &val, sizeof(val));
    jit_traverse_u32(mem, 8);
    jit_free(mem);
    return 0;
}
#endif
