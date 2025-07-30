// core_lib.h
#ifndef CORE_LIB_H
#define CORE_LIB_H

#include <stddef.h>
#include <stdint.h>

#define EXPORTED __attribute__((visibility("default")))

#ifdef __cplusplus
extern "C" {
#endif

EXPORTED void* alloc_mem(size_t size);
EXPORTED void  free_mem(void* ptr);
EXPORTED void  write_data(void* base, size_t offset, const void* data, size_t size);
EXPORTED void  dump_mem(void* base, size_t size);

typedef struct {
    uint8_t* data;
    size_t size;
    size_t top;
} Stack;

EXPORTED Stack* stack_create(size_t size);
EXPORTED void   stack_push(Stack* s, uint8_t val);
EXPORTED uint8_t stack_pop(Stack* s);
EXPORTED void   stack_free(Stack* s);

EXPORTED void write_output(const char* msg);
EXPORTED void read_input(char* buffer, size_t maxlen);

#ifdef __cplusplus
}
#endif

#endif
