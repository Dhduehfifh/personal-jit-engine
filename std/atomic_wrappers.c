#include <stdio.h>
#include "atomic_operation.h"

spinlock_t global_lock = { .value = 0 };

void atomic_lock_wrapper(void) {
    atomic_lock(&global_lock);
    printf("[dispatch] locked\n");
}

void atomic_unlock_wrapper(void) {
    atomic_unlock(&global_lock);
    printf("[dispatch] unlocked\n");
}

void atomic_pause_wrapper(void) {
    atomic_pause();
    printf("[dispatch] pause done\n");
}
