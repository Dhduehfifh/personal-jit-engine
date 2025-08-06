#pragma once
#include "bulitin_func.h"
#include "returns.h"
#include <stdint.h>

//tool
static inline uint64_t next_power_of_two(uint64_t n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
}

extern return_pool_t* current_return_pool = NULL;

void empty(uint64_t dummy) {
    return;
};
//do nothing

//std.io, alloc , release , terminate
//nece tools
static uint64_t arr_vtable_alloc(arr* a) {
    for (uint64_t i = 0; i < ARR_VTABLE_MAX; i++) {
        if (!arr_vtable[i].used){
            arr_vtable[i].used = true;
            arr_vtable[i].array = a;
            return i;
        }
    }
    return UINT64_MAX; // no useable 
}

//search arr* by id
static arr* arr_vtable_get(uint64_t id) {
    if (id >= ARR_VTABLE_MAX) return NULL;
    if (!arr_vtable[id].used) return NULL;
    return arr_vtable[id].array;
}

//release id by fake table
static void arr_vtable_free(uint64_t id) {
    if (id >= ARR_VTABLE_MAX) return;
    arr_vtable[id].used = false;
    arr_vtable[id].array = NULL;
}

// =====================
// 内建函数实现（已改为使用 return_pool_t）
// =====================
void alloc(uint64_t size, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx) {
        return_pool_alloc_data(current_return_pool, 0); // 占位返回值
        return;
    }
    uint64_t new_size = ctx->size + size;

    if (!ctx->data) {
        ctx->size = next_power_of_two(size);
        ctx->data = (uint64_t*)malloc(sizeof(uint64_t) * ctx->size);
        if (ctx->data) {
            memset(ctx->data, 0, sizeof(uint64_t) * ctx->size);
        } else {
            return_pool_alloc_data(current_return_pool, 0);
            return;
        }
    } else if (new_size > ctx->size) {
        uint64_t expanded_size = next_power_of_two(new_size);
        uint64_t* new_data = (uint64_t*)realloc(ctx->data, sizeof(uint64_t) * expanded_size);
        if (!new_data) {
            return_pool_alloc_data(current_return_pool, 0);
            return;
        }
        memset(new_data + ctx->size, 0, sizeof(uint64_t) * (expanded_size - ctx->size));
        ctx->data = new_data;
        ctx->size = expanded_size;
    }

    return_pool_alloc_data(current_return_pool, 1); // 成功
}

void release(uint64_t size, uint64_t ctx_ptr, uint64_t release_stack_head_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    uint64_t* release_stack_head = (uint64_t*)release_stack_head_ptr;
    if (!ctx || !ctx->data || !release_stack_head) {
        return_pool_alloc_data(current_return_pool, 0);
        return;
    }
    for (uint64_t i = 0; i < size; ++i) {
        uint64_t slot_index = *release_stack_head;
        ctx->data[slot_index] = slot_index; // 清空寄存器位置
        (*release_stack_head)++;
    }
    return_pool_alloc_data(current_return_pool, 1);
}

void terminate(uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (ctx && ctx->data) {
        free(ctx->data); // 真正释放内存回收给操作系统
        ctx->data = NULL;
        ctx->size = 0;
    }
    return_pool_alloc_data(current_return_pool, 1);
}

// VM 程序计数器（外部 VM 主循环会用到）
uint64_t vm_pc = 0;

// 纯比较，不跳转
void cmp_only(uint64_t place_a, uint64_t place_b, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    uint64_t val_a = ctx->data[place_a];
    uint64_t val_b = ctx->data[place_b];
    if (val_a == val_b) {
        // 存到全局比较标志
        ctx->data[0] = 1; // 这里用 data[0] 存 flag_eq（你也可以单独定义一个全局变量）
    } else {
        ctx->data[0] = 0;
    }
}

// 相等则跳转
void cmp_eq(uint64_t place_a, uint64_t place_b,
            uint64_t jump_eq_plc, uint64_t jump_not_eq_plc,
            uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    uint64_t val_a = ctx->data[place_a];
    uint64_t val_b = ctx->data[place_b];
    if (val_a == val_b) {
        vm_pc = jump_eq_plc;
    } else {
        vm_pc = jump_not_eq_plc;
    }
}

// 不相等则跳转
void cmp_not_eq(uint64_t place_a, uint64_t place_b,
                uint64_t jump_not_eq_plc, uint64_t jump_eq_plc,
                uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    uint64_t val_a = ctx->data[place_a];
    uint64_t val_b = ctx->data[place_b];
    if (val_a != val_b) {
        vm_pc = jump_not_eq_plc;
    } else {
        vm_pc = jump_eq_plc;
    }
}

// 直接跳转
void jump_to(uint64_t place) {
    vm_pc = place;
}

void add_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    ctx->data[plc_result] = ctx->data[plc_a] + ctx->data[plc_b];
}

void sub_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    ctx->data[plc_result] = ctx->data[plc_a] - ctx->data[plc_b];
}

void mul_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    ctx->data[plc_result] = ctx->data[plc_a] * ctx->data[plc_b];
}

void div_u64(uint64_t plc_a, uint64_t plc_b, uint64_t plc_result, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    if (ctx->data[plc_b] == 0) {
        ctx->data[plc_result] = 0; // 防止除零
        return;
    }
    ctx->data[plc_result] = ctx->data[plc_a] / ctx->data[plc_b];
}

// move: a -> b (同时清空 a)
void move(uint64_t plc_a, uint64_t plc_b, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    ctx->data[plc_b] = ctx->data[plc_a];
    ctx->data[plc_a] = 0;
}

// copy: a -> b (不清空 a)
void copy(uint64_t plc_a, uint64_t plc_b, uint64_t ctx_ptr) {
    arr* ctx = (arr*)ctx_ptr;
    if (!ctx || !ctx->data) return;
    ctx->data[plc_b] = ctx->data[plc_a];
}


// =====================
// Timer 实现
// =====================
static uint64_t timer_start_cycles = 0;
static uint64_t timer_end_cycles = 0;
static uint64_t timer_intervals[16][2]; // 多段计时存储
static uint64_t timer_count = 0;

static inline uint64_t get_tsc_value(void) {
    uint64_t hi, lo;
    __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

// 开始全局计时
void timer_start_now(uint64_t dummy) {
    timer_start_cycles = get_tsc_value();
}

// 结束计时并返回耗时
void timer_end_now(uint64_t dummy) {
    timer_end_cycles = get_tsc_value();
    uint64_t elapsed = timer_end_cycles - timer_start_cycles;
    return_pool_alloc_data(current_return_pool, elapsed);
}

// 多段计时开始
void timer_start_segment(uint64_t idx) {
    if (idx >= 16) return;
    timer_intervals[idx][0] = get_tsc_value();
}

// 多段计时结束并返回耗时
void timer_end_segment(uint64_t idx) {
    if (idx >= 16) return;
    timer_intervals[idx][1] = get_tsc_value();
    uint64_t elapsed = timer_intervals[idx][1] - timer_intervals[idx][0];
    return_pool_alloc_data(current_return_pool, elapsed);
}


// =====================
// VM Thread 控制（模式 1）
// =====================

typedef struct {
    uint64_t func_ptr; // 线程执行入口（VM 内部函数指针）
} vm_thread_arg_t;

static void* vm_thread_entry(void* arg) {
    vm_thread_arg_t* t = (vm_thread_arg_t*)arg;
    if (t && t->func_ptr) {
        void (*func)(void) = (void (*)(void))t->func_ptr;
        func();
    }
    free(t);
    return NULL;
}

// 启动新的 VM 执行线程
void vm_thread_spawn(uint64_t vm_func_ptr) {
    pthread_t tid;
    vm_thread_arg_t* arg = malloc(sizeof(vm_thread_arg_t));
    if (!arg) return;
    arg->func_ptr = vm_func_ptr;
    pthread_create(&tid, NULL, vm_thread_entry, arg);
    return_pool_alloc_data(current_return_pool, (uint64_t)tid);
}

// 等待线程结束
void vm_thread_join(uint64_t thread_id) {
    pthread_t tid = (pthread_t)thread_id;
    pthread_join(tid, NULL);
}

// 杀掉线程（强制终止）
void vm_thread_kill(uint64_t thread_id) {
    pthread_t tid = (pthread_t)thread_id;
    pthread_cancel(tid);
}

// 当前线程让出 CPU
void vm_thread_yield(uint64_t dummy) {
    sched_yield();
}


//tool sha 256 get first 8 bit transfer to uint64_t
static uint64_t file_hash64(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) return 0;

    SHA256_CTX ctx;
    SHA256_Init(&ctx);

    uint8_t buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        SHA256_Update(&ctx, buf, n);
    }
    fclose(f);

    uint8_t digest[SHA256_DIGEST_LENGTH];
    SHA256_Final(digest, &ctx);

    uint64_t hash_val;
    memcpy(&hash_val, digest, sizeof(uint64_t));
    return hash_val;
}

static external_lib_t *find_lib_by_hash(uint64_t hash) {
    for (size_t i = 0; i < external_lib_count; i++) {
        if (external_libs[i].hash == hash) {
            return &external_libs[i];
        }
    }
    return NULL;
}

// ===== 挂载动态库 =====
void builtin_mount_dynamic_lib(uint64_t path_ptr, uint64_t func_names_ptr, uint64_t func_count) {
    const char *path = (const char *)path_ptr;
    const char **func_names = (const char **)func_names_ptr;

    if (external_lib_count >= MAX_LIBS) return;

    uint64_t hash = file_hash64(path);
    if (!hash) return;

    if (find_lib_by_hash(hash)) {
        return_pool_alloc_data(current_return_pool, hash);
        return;
    }

    void *handle = dlopen(path, RTLD_NOW);
    if (!handle) return;

    external_lib_t *lib = &external_libs[external_lib_count];
    memset(lib, 0, sizeof(*lib));

    lib->hash = hash;
    lib->handle = handle;
    lib->func_count = func_count;

    for (size_t i = 0; i < func_count; i++) {
        void *sym = dlsym(handle, func_names[i]);
        lib->func_table[i] = sym;
    }

    external_lib_count++;

    return_pool_alloc_data(current_return_pool, hash); // 把库 hash 返回到 return_pool
}

// ===== 调用动态库函数 =====
void builtin_call_dynamic_func(uint64_t lib_hash, uint64_t func_index, uint64_t args_ptr, uint64_t argc) {
    external_lib_t *lib = find_lib_by_hash(lib_hash);
    if (!lib) return;

    if (func_index >= lib->func_count) return;
    void *func_ptr = lib->func_table[func_index];
    if (!func_ptr) return;

    typedef uint64_t (*func_t)(uint64_t, uint64_t, uint64_t, uint64_t,
                               uint64_t, uint64_t, uint64_t, uint64_t);

    func_t f = (func_t)func_ptr;

    uint64_t *args = (uint64_t *)args_ptr;
    uint64_t argv[MAX_FUNC_ARGS] = {0};
    if (argc > MAX_FUNC_ARGS) argc = MAX_FUNC_ARGS;
    for (size_t i = 0; i < argc; i++) argv[i] = args[i];

    uint64_t ret = f(argv[0], argv[1], argv[2], argv[3],
                     argv[4], argv[5], argv[6], argv[7]);

    return_pool_alloc_data(current_return_pool, ret);
}

