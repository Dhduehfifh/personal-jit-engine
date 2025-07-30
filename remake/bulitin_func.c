#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sched.h>
#include "bulitin_func.h"

// 定义 timer table（和大小）
int timer_table_size = 1024;
Timer* timer_table[1024] = {0};

// 可配置宽度
#ifndef VM_WORD
#define VM_WORD uint64_t
#endif

void empty() {
    NULL;
}

// 辅助函数：取不小于 x 的 2 次幂
static int next_power_of_two(int x) {
    int power = 1;
    while (power < x) power *= 2;
    return power;
}

// 初始化或扩容分配器
void alloc(int size, arr* ctx) {
    if (!ctx) return;

    int new_size = ctx->size + size;

    if (!ctx->data) {
        ctx->size = next_power_of_two(size);
        ctx->data = (VM_WORD*)malloc(sizeof(VM_WORD) * ctx->size);
        if (ctx->data) {
            memset(ctx->data, 0, sizeof(VM_WORD) * ctx->size);
        }
    } else if (new_size > ctx->size) {
        int expanded_size = next_power_of_two(new_size);
        VM_WORD* new_data = (VM_WORD*)realloc(ctx->data, sizeof(VM_WORD) * expanded_size);
        if (!new_data) return;
        memset(new_data + ctx->size, 0, sizeof(VM_WORD) * (expanded_size - ctx->size));
        ctx->data = new_data;
        ctx->size = expanded_size;
    }
}

// 将释放的 slot 的索引压入一个逻辑栈
void release(int size, arr* ctx, uint64_t* release_stack_head) {
    if (!ctx || !ctx->data || !release_stack_head) return;

    for (int i = 0; i < size; ++i) {
        uint64_t slot_index = *release_stack_head;
        ctx->data[slot_index] = (VM_WORD)(slot_index);
        (*release_stack_head)++;
    }
}

// 释放整个数组
void termiate(arr* ctx) {
    if (ctx && ctx->data) {
        free(ctx->data);
        ctx->data = NULL;
        ctx->size = 0;
    }
}

// 哈希函数
static inline size_t hash_id(uint64_t id) {
    return id % timer_table_size;
}

static uint64_t now_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);  // 修正为合法的 clock id
    return (uint64_t)ts.tv_sec * 1000000000ull + ts.tv_nsec;
}

// 查找计时器
static Timer* find_timer(uint64_t id) {
    size_t idx = hash_id(id);
    Timer* cur = timer_table[idx];
    while (cur) {
        if (cur->id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

// 创建计时器
Timer* new_timer(uint64_t id) {
    Timer* t = find_timer(id);
    if (t) return t;

    size_t idx = hash_id(id);
    Timer* new_t = (Timer*)malloc(sizeof(Timer));
    if (!new_t) return NULL;

    new_t->id = id;
    new_t->start_time = 0;
    new_t->elapsed_time = 0;
    new_t->next = timer_table[idx];
    timer_table[idx] = new_t;
    return new_t;
}

void start_timer(uint64_t id) {
    Timer* t = find_timer(id);
    if (!t) t = new_timer(id);
    if (!t) return;
    t->start_time = now_ns();
}

void end_timer(uint64_t id) {
    Timer* t = find_timer(id);
    if (!t || t->start_time == 0) return;

    uint64_t end_time = now_ns();
    if (end_time > t->start_time) {
        t->elapsed_time += (end_time - t->start_time);
    }
    t->start_time = 0;
}

uint64_t get_elapsed_time(uint64_t id) {
    Timer* t = find_timer(id);
    return t ? t->elapsed_time : 0;
}

void free_all_timers() {
    for (int i = 0; i < timer_table_size; i++) {
        Timer* cur = timer_table[i];
        while (cur) {
            Timer* next = cur->next;
            free(cur);
            cur = next;
        }
        timer_table[i] = NULL;
    }
}

#define MAX_THREADS 256

static pthread_t thread_pool[MAX_THREADS];
static int thread_used[MAX_THREADS] = {0};

// 假设 vm_execute_core 存在
extern void vm_execute_core(uint64_t* core);
extern uint64_t* current_page;

void* thread_entry(void* arg) {
    uint64_t* core = (uint64_t*)arg;
    vm_execute_core(core);
    return NULL;
}

void vm_thread_spawn(void) {
    uint64_t* core_to_run = (uint64_t*)current_page[1];

    for (int i = 0; i < MAX_THREADS; ++i) {
        if (!thread_used[i]) {
            pthread_create(&thread_pool[i], NULL, thread_entry, (void*)core_to_run);
            thread_used[i] = 1;
            break;
        }
    }
}

void vm_thread_pause(void) {
    sleep(0);  // 等效于 yield
}

void vm_thread_resume(void) {
    // 占位，不实际控制线程
}

void vm_thread_kill(void) {
    uint64_t tid = current_page[1];
    if (tid < MAX_THREADS && thread_used[tid]) {
        pthread_cancel(thread_pool[tid]);
        thread_used[tid] = 0;
    }
}

void vm_thread_yield(void) {
    sched_yield();
}
