// memory.c
#include <sys/mman.h>
#include "memory.h"
#include "jit_panic.h"

#define MAX_PAGES 1024

typedef struct {
    void*   start;
    uint8_t is_used;
    uint8_t gc_tag;
} PageEntry;

static PageEntry page_table[MAX_PAGES];
static uint8_t* heap_base;

void init_memory_system() {
    heap_base = mmap(NULL, MAX_PAGES * PAGE_SIZE, 
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (heap_base == MAP_FAILED) {
        jit_panic(MEM_INIT_FAIL);
    }
}

// 修改jit_alloc_page和jit_free_page签名
void jit_alloc_page(JitContext* ctx) {
    void* p = _internal_alloc();  // 实际分配函数
    ctx->x[0] = (uint64_t)p;      // 返回值存x0
}

void jit_free_page(JitContext* ctx) {
    void* ptr = (void*)ctx->x[1]; // 参数从x1读取
    _internal_free(ptr);
}

void jit_mark_page(void* ptr, uint8_t tag) {
    size_t idx = ((uint8_t*)ptr - heap_base) / PAGE_SIZE;
    if (idx < MAX_PAGES) {
        page_table[idx].gc_tag = tag;
    }
}