#ifndef BULITIN_FUNC_H
#define BULITIN_FUNC_H

#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// 先定义结构体，避免类型未声明
typedef struct Timer {
    uint64_t id;
    uint64_t start_time;
    uint64_t elapsed_time;
    struct Timer* next;  // 哈希冲突链表
} Timer;

typedef struct {
    uint64_t* data;
    int size;
} arr;

// 外部计时器表（实际定义在 .c 中）
extern int timer_table_size;
extern Timer* timer_table[1024];

void empty();

void alloc(int size, arr* ctx);
void release(int size, arr* ctx, uint64_t* release_stack_head);
void termiate(arr* ctx);

extern void compare(uint64_t a, uint64_t b);
extern void jump_if_eq(uint64_t* target);
extern void jump_if_neq(uint64_t* target);
extern void jump(uint64_t* target);

void start_timer(uint64_t id);
void end_timer(uint64_t id);
uint64_t get_elapsed_time(uint64_t id);
void free_all_timers(void);

// 多线程接口
void vm_thread_spawn(void);   // 创建新线程
void vm_thread_pause(void);   // 暂停当前线程
void vm_thread_resume(void);  // 恢复指定线程（占位）
void vm_thread_kill(void);    // 杀死指定线程
void vm_thread_yield(void);   // 当前线程让出 CPU 时间片

#endif
