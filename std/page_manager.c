#include "page_manager.h"
#include <sys/mman.h>
#include <stdlib.h>

void init_page_buffer(PageBuffer* page, size_t capacity) {
    page->buffer = mmap(NULL, capacity, 
                       PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    page->capacity = capacity;
    page->lock = 0;
}

void safe_write(PageBuffer* page, const uint8_t* data, size_t len) {
    if (!page || !data || len == 0 || len > page->capacity) return;
    asm_buffer_write(page, data, len);
}

void destroy_page_buffer(PageBuffer* page) {
    if (page && page->buffer) {
        munmap(page->buffer, page->capacity);
        page->buffer = NULL;
    }
}