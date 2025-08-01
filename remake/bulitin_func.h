#ifndef BULITIN_FUNC_H
#define BULITIN_FUNC_H

#include <stdint.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//default value of debug otpt
bool if_debug;

typedef struct Timer {
    uint64_t id;
    uint64_t start_time;
    uint64_t elapsed_time;
    struct Timer* next;
} Timer;

typedef struct {
    uint64_t* data;
    int size;
} arr;

extern int timer_table_size;
extern Timer* timer_table[1024];

// 这里只声明，不定义
extern uint64_t* current_page;

// 声明 VM 核心执行函数
void vm_execute_core(uint64_t* core);

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

void vm_thread_spawn(void);
void vm_thread_pause(void);
void vm_thread_resume(void);
void vm_thread_kill(void);
void vm_thread_yield(void);

#endif
