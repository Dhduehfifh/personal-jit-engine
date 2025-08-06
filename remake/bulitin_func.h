#ifndef BULITIN_FUNC_H
#define BULITIN_FUNC_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <openssl/sha.h>
#include "returns.h"

// default value of debug output
extern bool if_debug;


// =====================
// 计时器结构
// =====================
typedef struct Timer {
    uint64_t id;
    uint64_t start_time;
    uint64_t elapsed_time;
    struct Timer* next;
} Timer;

// =====================
// 数组上下文（VM 内部数据块）
// =====================
typedef struct {
    uint64_t* data;
    uint64_t size;
} arr;
#define ARR_VTABLE_MAX 1024

typedef struct {
    bool used;
    arr* array;
} arr_vtable_entry_t;

// 全局虚表（后面可改成 per-VM 的）
static arr_vtable_entry_t arr_vtable[ARR_VTABLE_MAX];

// =====================
// VM 数据槽（保证 8 字节对齐）
// =====================
typedef struct __attribute__((packed, aligned(8))) {
    uint64_t v[4]; // 可根据需要扩展，这里一个槽最多存 4 个 uint64_t
} vm_data_slot_t;

extern int timer_table_size;
extern Timer* timer_table[1024];

// 当前执行页（VM 内部指令 + 数据混合区）
extern uint64_t* current_page;

// VM 核心执行函数
void vm_execute_core(uint64_t core_ptr);

// 内建函数接口（参数全部统一为 uint64_t，返回值通过 returns 模块管理，函数返回void）
void empty(uint64_t dummy);
extern return_pool_t* current_return_pool; // 当前 VM 的返回池

void alloc(uint64_t size, uint64_t ctx_ptr);
void release(uint64_t size, uint64_t ctx_ptr, uint64_t release_stack_head_ptr);
void terminate(uint64_t ctx_ptr);

// 内建函数（参数统一为 uint64_t，返回值通过 returns 模块）

extern uint64_t vm_pc;

void cmp_only(uint64_t place_a, uint64_t place_b, uint64_t ctx_ptr);
void cmp_eq(uint64_t place_a, uint64_t place_b, uint64_t jump_eq_plc, uint64_t jump_not_eq_plc, uint64_t ctx_ptr);
void cmp_not_eq(uint64_t place_a, uint64_t place_b, uint64_t jump_not_eq_plc, uint64_t jump_eq_plc, uint64_t ctx_ptr);
void jump_to(uint64_t place);




void add_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr);
void sub_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr);
void mul_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr);
void div_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr);

void move(uint64_t plc_a, uint64_t plc_b, uint64_t ctx_ptr);
void copy(uint64_t plc_a, uint64_t plc_b, uint64_t ctx_ptr);

void get_tsc(void); // 返回值通过 returns 管理

// =====================
// VM Timer （简化版）
// =====================
void timer_start_now(uint64_t dummy); // 开始计时
void timer_end_now(uint64_t dummy);   // 结束计时并返回耗时（cycles）
void timer_start_segment(uint64_t idx); // 多段计时开始
void timer_end_segment(uint64_t idx);   // 多段计时结束并返回耗时

// =====================
// VM Thread 控制（模式 1: VM 控制 VM）
// =====================
void vm_thread_spawn(uint64_t vm_func_ptr);  // 启动新的 VM 执行线程
void vm_thread_join(uint64_t thread_id);     // 等待线程结束
void vm_thread_kill(uint64_t thread_id);     // 杀掉线程
void vm_thread_yield(uint64_t dummy);        // 当前线程让出 CPU

#define MAX_LIBS        32
#define MAX_LIB_FUNCS   128
#define MAX_FUNC_ARGS   8

typedef struct {
    uint64_t hash;
    void *handle;
    void *func_table[MAX_LIB_FUNCS];
    size_t func_count;
} external_lib_t;

static external_lib_t external_libs[MAX_LIBS];
static size_t external_lib_count = 0;

void builtin_mount_dynamic_lib(uint64_t path_ptr, uint64_t func_names_ptr, uint64_t func_count);
void builtin_call_dynamic_func(uint64_t lib_hash, uint64_t func_index, uint64_t args_ptr, uint64_t argc);



#endif
