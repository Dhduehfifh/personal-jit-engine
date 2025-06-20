// atomic_operation.c
#include "atomic_operation.h"

int atomic_lock(spinlock_t* lock) {
    while (!atomic_lock_asm(&lock->value)) {
        atomic_pause_asm();
    }
    return 1;
}

void atomic_unlock(spinlock_t* lock) {
    atomic_unlock_asm(&lock->value);
}

int atomic_compare_and_swap(int* addr, int expected, int desired) {
    return atomic_cas_asm(addr, expected, desired);
}

void atomic_pause() {
    atomic_pause_asm();
}
