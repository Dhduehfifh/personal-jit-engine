// atomic_operation.h
#ifndef ATOMIC_OPERATION_H
#define ATOMIC_OPERATION_H

typedef struct {
    volatile int value;
} spinlock_t;

int atomic_lock(spinlock_t* lock);
void atomic_unlock(spinlock_t* lock);
int atomic_compare_and_swap(int* addr, int expected, int desired);
void atomic_pause();

// 汇编声明
extern int  atomic_lock_asm(int* lock);
extern void atomic_unlock_asm(int* lock);
extern int  atomic_cas_asm(int* addr, int expected, int desired);
extern void atomic_pause_asm();

#endif
