#include "studio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#define MAX_DISPATCH 256

typedef struct {
    char name[64];
    void* fn_ptr;
} DispatchEntry;

static void* current_handle = NULL;
static DispatchEntry dispatch_table[MAX_DISPATCH];
static int dispatch_count = 0;

int lib_studio_init(void) {
    current_handle = NULL;
    dispatch_count = 0;
    return 0;
}

void lib_studio_cleanup(void) {
    if (current_handle) {
        dlclose(current_handle);
        current_handle = NULL;
    }
    dispatch_count = 0;
}

int lib_studio_switch_library(const char* so_path) {
    if (!so_path) return -1;
    if (current_handle) {
        dlclose(current_handle);
        current_handle = NULL;
    }

    current_handle = dlopen(so_path, RTLD_LAZY);
    if (!current_handle) {
        fprintf(stderr, "[lib_studio] dlopen failed: %s\n", dlerror());
        return -2;
    }

    // 载入符号前清空旧表
    dispatch_count = 0;

    return 0;
}

int lib_studio_register_handler(void* fn_ptr, const char* name) {
    if (dispatch_count >= MAX_DISPATCH) return -1;
    strncpy(dispatch_table[dispatch_count].name, name, 63);
    dispatch_table[dispatch_count].name[63] = '\0'; // 安全结尾
    dispatch_table[dispatch_count].fn_ptr = fn_ptr;
    return dispatch_count++;
}

void* lib_studio_find_handler_by_name(const char* name) {
    for (int i = 0; i < dispatch_count; ++i) {
        if (strcmp(dispatch_table[i].name, name) == 0) {
            return dispatch_table[i].fn_ptr;
        }
    }
    return NULL;
}

int lib_studio_load_config(const char* config_path) {
    if (!current_handle || !config_path) return -1;
    FILE* f = fopen(config_path, "r");
    if (!f) return -2;

    char line[128];
    while (fgets(line, sizeof(line), f)) {
        char* newline = strchr(line, '\n');
        if (newline) *newline = '\0';

        void* sym = dlsym(current_handle, line);
        if (!sym) {
            fprintf(stderr, "[lib_studio] dlsym failed for %s: %s\n", line, dlerror());
            continue;
        }
        lib_studio_register_handler(sym, line);
    }

    fclose(f);
    return 0;
}

void lib_studio_init_builtin_dispatch(void) {
    // 如果你有默认库路径和默认config，可以在这里加载
    // 例如：
    // lib_studio_switch_library("./libdefault.so");
    // lib_studio_load_config("./default.config");
}

// 由汇编层调用的函数
int lib_studio_dispatch_entry(int code, void* ctx) {
    if (code < 0 || code >= dispatch_count) return -1;
    typedef int (*fn_t)(void*);
    fn_t fn = (fn_t)dispatch_table[code].fn_ptr;
    if (!fn) return -2;
    return fn(ctx);
}
