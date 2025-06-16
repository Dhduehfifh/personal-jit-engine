#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>

#ifdef _WIN32
#include <windows.h>
#define EXPORT __declspec(dllexport)
#else
#include <unistd.h>
#define EXPORT __attribute__((visibility("default")))
#endif

EXPORT void* malloc_sys(size_t size) {
    return malloc(size);
}

EXPORT void free_sys(void* ptr) {
    free(ptr);
}

EXPORT void* aligned_alloc_sys(size_t size, size_t align) {
    void* ptr = NULL;
#ifdef _WIN32
    ptr = _aligned_malloc(size, align);
#else
    posix_memalign(&ptr, align, size);
#endif
    return ptr;
}

EXPORT int output_sys(const char* msg) {
#ifdef _WIN32
    DWORD written;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    WriteConsoleA(h, msg, (DWORD)strlen(msg), &written, NULL);
    return (int)written;
#else
    return write(1, msg, strlen(msg));
#endif
}

EXPORT void output_sys_int(uintptr_t val) {
    char buf[64];
    snprintf(buf, sizeof(buf), "%" PRIuPTR "\n", val);
    output_sys(buf);
}

EXPORT void exit_sys(int code) {
    exit(code);
}