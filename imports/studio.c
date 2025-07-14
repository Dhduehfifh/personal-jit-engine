#include "studio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

static void* current_handle = NULL;

int studio_init(void) {
    current_handle = NULL;
    return 0;
}

void studio_cleanup(void) {
    if (current_handle) {
        dlclose(current_handle);
        current_handle = NULL;
    }
}

// 切换加载库，卸载旧库，加载新库
int studio_switch_library(const char* so_path) {
    if (!so_path) return -1;

    // 先卸载旧库
    if (current_handle) {
        dlclose(current_handle);
        current_handle = NULL;
    }

    // 加载新库
    current_handle = dlopen(so_path, RTLD_LAZY);
    if (!current_handle) {
        fprintf(stderr, "[studio] dlopen failed: %s\n", dlerror());
        return -2;
    }
    return 0;
}

// 通过函数名调用函数，arg可为NULL
int studio_call_function(const char* func_name, void* arg) {
    if (!current_handle || !func_name) return -1;

    typedef int (*func_t)(void*);
    dlerror(); // 清除旧错误

    func_t fn = (func_t)dlsym(current_handle, func_name);
    char* err = dlerror();
    if (err != NULL) {
        fprintf(stderr, "[studio] dlsym failed: %s\n", err);
        return -2;
    }

    return fn(arg);
}
