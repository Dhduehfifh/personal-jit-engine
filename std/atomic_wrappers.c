#include <stdio.h>
#include "atomic_operation.h"

spinlock_t global_lock = { .value = 0 };
int global_lock_value = 0; // 提供给 main 打印

void atomic_lock_wrapper(void* ctx) {
    atomic_lock(&global_lock);
    global_lock_value = global_lock.value;
    printf("[dispatch] locked\n");
}

void atomic_unlock_wrapper(void* ctx) {
    atomic_unlock(&global_lock);
    global_lock_value = global_lock.value;
    printf("[dispatch] unlocked\n");
}

void atomic_pause_wrapper(void* ctx) {
    atomic_pause();
    printf("[dispatch] pause done\n");
}
