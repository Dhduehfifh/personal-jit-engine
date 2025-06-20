#include <stdio.h>
#include "dispatch_table.h"
#include "atomic_operation.h"
#include "dispatch.h"

extern spinlock_t global_lock;

int main() {
    init_dispatch_table();

    printf("Before lock: %d\n", global_lock.value);

    // 测试分发执行
    dispatch_table[0x10](); // atomic_lock_wrapper
    dispatch_table[0x12](); // atomic_pause_wrapper
    dispatch_table[0x11](); // atomic_unlock_wrapper

    printf("After unlock: %d\n", global_lock.value);
    return 0;
}
