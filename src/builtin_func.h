#pragma once
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

//========空实现，专门为了那种return NULL的场景设计 ========
void noop_wrapper(void* ctx);

// ======== JitContext: JIT 内存分配上下文 ========
typedef struct {
    void* code_page;
    size_t code_offset;
    size_t code_page_size;
    size_t requested_alloc_size;
} JitContext;

// ======== 字段类型定义 ========
typedef enum {
    FIELD_INT,
    FIELD_BOOL,
    FIELD_PTR,
    FIELD_RAW,
    FIELD_FUNCTION
} FieldType;

// ======== 字段结构体（每个结构字段定义）=======
typedef struct {
    char* name;
    FieldType type;
    int offset;
    int size;
    void* default_handler;
} StructField;

// ======== 结构体定义结构体 ========
typedef struct {
    char* name;
    int is_union;
    int total_size;
    int field_count;
    StructField* fields;
} StructDef;

// ======== 结构体实例 ========
typedef struct {
    const StructDef* def;
    void* data;
    void** field_ptrs;
} StructInstance;

// ======== 自旋锁结构体 ========
typedef struct {
    volatile int value;
} spinlock_t;

// ======== 内存操作函数 ========  
void jit_init(JitContext* ctx);                // ptr = 0x01
void* jit_alloc_page(JitContext* ctx);         // ptr = 0x02
void jit_free(JitContext* ctx);                // ptr = 0x03

// ======== Struct 定义相关函数 ========
char* safe_strdup(const char* s);
StructDef* create_struct_def(const char* name, int is_union);
void add_field_to_struct(StructDef* def, const char* field_name, FieldType type, int size_if_raw);
void finalize_struct(StructDef* def);
int get_field_offset(const StructDef* def, const char* field_name);
void destroy_struct_def(StructDef* def);

// ======== Struct 实例相关函数 ========
StructInstance* create_struct_instance(JitContext* ctx, const StructDef* def);
void destroy_struct_instance(StructInstance* inst);

// ======== 函数字段支持 ========
typedef void (*FunctionHandler)(void* ctx);
void add_function_field(StructDef* def, const char* name, FunctionHandler default_fn);
void update_function_handler(StructField* field, FunctionHandler new_fn);
void call_function_field(void* instance_data, StructField* field);
int call_function_by_name(void* instance_data, const StructDef* def, const char* name);

// ======== 原子操作（C 封装） ========
int atomic_lock(spinlock_t* lock);
void atomic_unlock(spinlock_t* lock);
int atomic_compare_and_swap(int* addr, int expected, int desired);
void atomic_pause();

// ======== 原子操作（汇编实现） ========
extern int atomic_lock_asm(int* lock);
extern void atomic_unlock_asm(int* lock);
extern int atomic_cas_asm(int* addr, int expected, int desired);
extern void atomic_pause_asm(); //dont use this right now

// ======== Panic 处理机制 ========
typedef enum {
    JIT_PANIC_NONE = 0,
    JIT_PANIC_MEM_INIT_FAIL = 0xDEAD0001,
    JIT_PANIC_OUT_OF_MEMORY = 0xDEAD0002,
    JIT_PANIC_INVALID_FREE = 0xDEAD0003,
    JIT_PANIC_DOUBLE_FREE = 0xDEAD0004,
    JIT_PANIC_ILLEGAL_OPCODE = 0xDEAD0005,
    JIT_PANIC_STACK_OVERFLOW = 0xDEAD0006,
    JIT_PANIC_GC_CORRUPTION = 0xDEAD0007
} JitPanicCode;
typedef struct {
    uint32_t code;
    const char* msg;
    const char* file;
    int line;
    time_t timestamp;
} JitPanicRecord;

void jit_panic(uint32_t code);
typedef void(*JitPanicHandeler)(uint32_t code);
void set_jit_panic_handler(JitPanicHandeler handler);
void jit_panic_cleanup();

extern JitPanicRecord* panic_records;
extern size_t panic_count;
extern int panic_enabled;

//========logic control ========

extern void* logic_if_zero_wrapper(void* ctx);
extern void* logic_if_not_zero_wrapper(void* ctx);

//========math functions ========
void* math_add_wrapper(void* ctx);
void* math_sub_wrapper(void* ctx);
void* math_mul_wrapper(void* ctx);
void* math_div_wrapper(void* ctx);
void* math_sqrt_wrapper(void* ctx);
void* math_pow_wrapper(void* ctx);