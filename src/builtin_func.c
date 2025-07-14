#include "builtin_func.h"
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <pthread.h>

//======== NULL ========
 

//================= Memory ===================

// 初始化 JIT 上下文
void jit_init(JitContext* ctx) {
    memset(ctx, 0, sizeof(JitContext));
}

// 分配一个内存页（可执行权限）
void* jit_alloc_page(JitContext* ctx) {
    if (!ctx) {
        fprintf(stderr, "[JIT] NULL context\n");
        return NULL;
    }

    if (ctx->code_page) {
        jit_free(ctx);  // 重新分配前先释放原页
    }

    size_t size = ctx->requested_alloc_size;
    if (size == 0) size = 4096;  // 默认页大小

    void* page = mmap(NULL, size,
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      -1, 0);
    if (page == MAP_FAILED) {
        perror("[JIT] mmap failed");
        return NULL;
    }

    ctx->code_page = page;
    ctx->code_offset = 0;
    ctx->code_page_size = size;
    return page;
}

// 释放已分配页
void jit_free(JitContext* ctx) {
    if (ctx && ctx->code_page) {
        munmap(ctx->code_page, ctx->code_page_size);
        ctx->code_page = NULL;
        ctx->code_offset = 0;
        ctx->code_page_size = 0;
        ctx->requested_alloc_size = 0;
    }
}

//================= Struct Definition ===================

// 安全字符串复制（heap 分配）
char* safe_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) memcpy(copy, s, len + 1);
    return copy;
}

// 创建结构定义（可为 union）
StructDef* create_struct_def(const char* name, int is_union) {
    StructDef* def = malloc(sizeof(StructDef));
    def->name = safe_strdup(name);
    def->is_union = is_union;
    def->total_size = 0;
    def->field_count = 0;
    def->fields = NULL;
    return def;
}

// 向结构体添加字段（支持基本类型和 RAW 数据）
void add_field_to_struct(StructDef* def, const char* name, FieldType type, int raw_size) {
    if (!def || !name) return;
    int size = 0;
    switch (type) {
        case FIELD_INT: size = 4; break;
        case FIELD_BOOL: size = 1; break;
        case FIELD_PTR: size = 8; break;
        case FIELD_RAW: size = raw_size; break;
        default: return;
    }

    StructField field;
    field.name = safe_strdup(name);
    field.type = type;
    field.size = size;
    field.default_handler = NULL;
    field.offset = def->is_union ? 0 : 
        (def->field_count > 0 ? 
         def->fields[def->field_count - 1].offset + def->fields[def->field_count - 1].size : 0);

    def->fields = realloc(def->fields, sizeof(StructField) * (def->field_count + 1));
    def->fields[def->field_count++] = field;
}

// 计算结构体实际大小（结构完成后必须调用）
void finalize_struct(StructDef* def) {
    int max_end = 0;
    for (int i = 0; i < def->field_count; ++i) {
        int end = def->fields[i].offset + def->fields[i].size;
        if (end > max_end) max_end = end;
    }
    def->total_size = max_end;
}

// 获取某字段在结构体内的偏移
int get_field_offset(const StructDef* def, const char* name) {
    for (int i = 0; i < def->field_count; ++i) {
        if (strcmp(def->fields[i].name, name) == 0)
            return def->fields[i].offset;
    }
    return -1;
}

// 销毁结构定义（释放内存）
void destroy_struct_def(StructDef* def) {
    for (int i = 0; i < def->field_count; ++i)
        free(def->fields[i].name);
    free(def->fields);
    free(def->name);
    free(def);
}

//================= Struct Instance ===================

// 创建结构实例（在 JIT 分配页中分配空间）
StructInstance* create_struct_instance(JitContext* ctx, const StructDef* def) {
    if (!ctx || !def) return NULL;
    if (!ctx->code_page && !jit_alloc_page(ctx)) return NULL;
    if (ctx->code_offset + def->total_size > ctx->code_page_size) return NULL;

    StructInstance* inst = malloc(sizeof(StructInstance));
    inst->def = def;
    inst->data = (char*)ctx->code_page + ctx->code_offset;
    inst->field_ptrs = malloc(sizeof(void*) * def->field_count);
    ctx->code_offset += def->total_size;
    memset(inst->data, 0, def->total_size);

    for (int i = 0; i < def->field_count; ++i) {
        inst->field_ptrs[i] = inst->data + def->fields[i].offset;
    }

    return inst;
}

// 销毁结构体实例（不释放内部数据页）
void destroy_struct_instance(StructInstance* inst) {
    if (inst) {
        free(inst->field_ptrs);
        free(inst);
    }
}

//================= Function Field ===================

// 添加函数字段（FIELD_FUNCTION）
void add_function_field_wrapper(void* ctx) {
    if (!ctx) return;
    void** args = (void**)ctx;
    StructDef* def = (StructDef*)args[0];
    const char* name = (const char*)args[1];
    FunctionHandler fn = (FunctionHandler)args[2];

    StructField field;
    field.name = safe_strdup(name);
    field.type = FIELD_FUNCTION;
    field.size = sizeof(void*);
    field.default_handler = fn;
    field.offset = (def->field_count > 0) ?
        def->fields[def->field_count - 1].offset + def->fields[def->field_count - 1].size : 0;

    def->fields = realloc(def->fields, sizeof(StructField) * (def->field_count + 1));
    def->fields[def->field_count++] = field;
}

void update_function_handler_wrapper(void* ctx) {
    if (!ctx) return;
    void** args = (void**)ctx;
    StructField* field = (StructField*)args[0];
    FunctionHandler new_fn = (FunctionHandler)args[1];

    if (field && field->type == FIELD_FUNCTION) {
        field->default_handler = new_fn;
    }
}

void call_function_field_wrapper(void* ctx) {
    if (!ctx) return;
    void** args = (void**)ctx;
    void* instance_data = args[0];
    StructField* field = (StructField*)args[1];

    if (!field || field->type != FIELD_FUNCTION) return;

    FunctionHandler fn = *(FunctionHandler*)((char*)instance_data + field->offset);
    if (field->default_handler) {
        ((FunctionHandler)(field->default_handler))(instance_data);
    } else if (fn) {
        fn(instance_data);
    }
}

void call_function_by_name_wrapper(void* ctx) {
    if (!ctx) return;
    void** args = (void**)ctx;
    void* instance_data = args[0];
    const StructDef* def = (const StructDef*)args[1];
    const char* name = (const char*)args[2];

    for (int i = 0; i < def->field_count; ++i) {
        StructField* f = &def->fields[i];
        if (f->type == FIELD_FUNCTION && strcmp(f->name, name) == 0) {
            call_function_field(instance_data, f);
            return;
        }
    }
}


//================= Atomic ===================

int atomic_lock(spinlock_t* lock) {
    while (!atomic_lock_asm(&lock->value)) atomic_pause_asm();
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

//================= Panic ===================
JitPanicRecord* panic_records = NULL;
size_t panic_count = 0;
int panic_enabled = 1;

void set_jit_panic_handler(JitPanicHandeler handler) {
    return NULL;
}

void jit_panic(uint32_t code) {
    if (!panic_enabled) return;  // 全局开关检查

    const char* msg;
    switch (code) {
        case 0xDEAD0001: msg = "[PANIC] 内存初始化失败"; break;
        case 0xDEAD0002: msg = "[PANIC] 内存耗尽"; break;
        case 0xDEAD0003: msg = "[PANIC] 非法指针释放"; break;
        case 0xDEAD0004: msg = "[PANIC] 双重释放"; break;
        case 0xDEAD0005: msg = "[PANIC] 非法操作码"; break;
        case 0xDEADBEEF: msg = "[PANIC] 自定义错误"; break;
        default: msg = "[PANIC] 未知错误"; break;
    }

    JitPanicRecord record = {
        .code = code,
        .msg = msg,
        .file = NULL,
        .line = 0,
        .timestamp = time(NULL)
    };

    JitPanicRecord* new_records = realloc(panic_records, (panic_count + 1) * sizeof(JitPanicRecord));
    if (new_records) {
        panic_records = new_records;
        panic_records[panic_count++] = record;
    }

    fprintf(stderr, "%s (code=0x%X)\n", msg, code);
    abort();
}

void jit_panic_cleanup() {
    if (panic_count > 0) {
        fprintf(stderr, "\n=== JIT错误报告 ===\n");
        for (size_t i = 0; i < panic_count; i++) {
            char time_str[20];
            strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&panic_records[i].timestamp));
            fprintf(stderr, "[%s] %s (0x%X)\n", time_str, panic_records[i].msg, panic_records[i].code);
        }
    }
    free(panic_records);
    panic_records = NULL;
    panic_count = 0;
}

// ======== Math ========

static inline int* get_args(void* ctx) {
    return (int*)ctx;
}

void* math_add_wrapper(void* ctx) {
    float* input = (float*)ctx;
    static float result;
    result = input[0] + input[1];
    return &result;
}

void* math_sub_wrapper(void* ctx) {
    float* input = (float*)ctx;
    static float result;
    result = input[0] - input[1];
    return &result;
}

void* math_mul_wrapper(void* ctx) {
    float* input = (float*)ctx;
    static float result;
    result = input[0] * input[1];
    return &result;
}

void* math_div_wrapper(void* ctx) {
    float* input = (float*)ctx;
    static float result;
    if (input[1] == 0.0f) {
        result = 0.0f;
    } else {
        result = input[0] / input[1];
    }
    return &result;
}

void* math_sqrt_wrapper(void* ctx) {
    float* input = (float*)ctx;
    static float result;
    result = sqrt(input[0]);
    return &result;
}

void* math_pow_wrapper(void* ctx) {
    float* input = (float*)ctx;
    static float result;
    result = pow(input[0], input[1]);
    return &result;
}
