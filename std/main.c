#include <stdio.h>
#include "dispatch_table.h"
#include "atomic_operation.h"

extern void atomic_lock_wrapper(void);
extern void atomic_unlock_wrapper(void);
extern void atomic_pause_wrapper(void);

extern int global_lock_value;

int main() {
    init_dispatch_table();

    printf("Before lock: %d\n", global_lock_value);

    trigger_handler(0x10); // atomic_lock_wrapper
    trigger_handler(0x12); // atomic_pause_wrapper
    trigger_handler(0x11); // atomic_unlock_wrapper

    printf("After unlock: %d\n", global_lock_value);
    return 0;
}
