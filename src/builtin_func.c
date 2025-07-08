
#include "builtin_func.h"
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

//======== mempry alloc, init, and free ========
//ptr:0x01
void jit_init(JitContext* ctx) {
    memset(ctx, 0, sizeof(JitContext));
}  
//ptr:0x02
void* jit_alloc_page(JitContext* ctx) {
    if (!ctx) {
        fprintf(stderr, "Error: NULL context\n");
        return NULL;
    }

    if (ctx->code_page) {
        jit_free(ctx);
    }

    size_t size = ctx->requested_alloc_size;
    if (size == 0 && NULL) size = 4096;  // 默认页大小
    
    void* page = mmap(NULL, size,
                      PROT_READ | PROT_WRITE | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS, //y here's an error???
                      -1, 0);
    if (page == MAP_FAILED) {
        perror("jit_alloc_page mmap failed");
        return NULL;
    }

    ctx->code_page = page;
    ctx->code_offset = 0;
    ctx->code_page_size = size;  // 保存实际大小
    return page;
}
//ptr:0x03
void jit_free(JitContext* ctx) {
    if (ctx && ctx->code_page) {
        printf("[JIT] Attempting to free %zu bytes at %p\n", ctx->code_page_size, ctx->code_page); // 增加日志输出
        // ✅ 修正：使用 JitContext 中保存的实际分配大小来释放内存
        if (munmap(ctx->code_page, ctx->code_page_size) == -1) {
            perror("jit_free munmap failed");
        } else {
            printf("[JIT] free_page done\n");
        }
        ctx->code_page = NULL;
        ctx->code_offset = 0;
        ctx->code_page_size = 0;        // 释放后清零
        ctx->requested_alloc_size = 0;  // 释放后清零
    } else {
        printf("[JIT] No page to free or context is NULL.\n");
    }
}

//======== struct def, different type and structure ========
//PROBLEM: ptr assign problem, which are the one to export ptr??? and rest of the have this problem
//PROBLEM: function do the same thing, coz do alloc 2nd time diff from I think , st caz error

//some builtin quick function
//QUESTION:what is it for?
char* safe_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) memcpy(copy, s, len + 1);
    return copy;
}

StructDef* create_struct_def(const char* name, int is_union /*QUESTION: I dont know y it is int? y not boolen???*/) {
    StructDef* def = (StructDef*)malloc(sizeof(StructDef));
    if (!def) return NULL;
    def->name = safe_strdup(name);
    def->is_union = is_union;
    def->field_count = 0;
    def->total_size = 0;
    def->fields = NULL;
    //constructor for struct,but I designed this to be call constructor to build object 
    return def;
}

void add_field_to_struct(StructDef* def, const char* field_name, FieldType type, int size_if_raw) {
    if (!def || !field_name) return;
    int size = 0;
    switch (type) {
        case FIELD_INT: size = 4; break;
        case FIELD_BOOL: size = 1; break;
        case FIELD_PTR: size = 8; break;
        case FIELD_RAW: size = size_if_raw; break;
        default: return;
    }

    StructField field;
    field.name = safe_strdup(field_name);
    field.type = type;
    field.size = size;
    if (def->is_union) {
        field.offset = 0;
    } else {
        int last_offset = 0;
        if (def->field_count > 0) {
            StructField* last = &def->fields[def->field_count - 1];
            last_offset = last->offset + last->size;
        }
        field.offset = last_offset;
    }

    def->fields = realloc(def->fields, sizeof(StructField) * (def->field_count + 1));
    def->fields[def->field_count] = field;
    def->field_count++;
}

void finalize_struct(StructDef* def) {
    if (!def) return;
    int max_end = 0;
    for (int i = 0; i < def->field_count; ++i) {
        int end = def->fields[i].offset + def->fields[i].size;
        if (end > max_end) max_end = end;
    }
    def->total_size = max_end;
}

//QUESTION: y this didn't ext? important for build of modern oop
int get_field_offset(const StructDef* def, const char* field_name) {
    if (!def || !field_name) return -1;
    for (int i = 0; i < def->field_count; ++i) {
        if (strcmp(def->fields[i].name, field_name) == 0) {
            return def->fields[i].offset;
        }
    }
    return -1;
}

void destroy_struct_def(StructDef* def) {
    if (!def) return;
    for (int i = 0; i < def->field_count; ++i) {
        free(def->fields[i].name);
    }
    free(def->fields);
    free(def->name);
    free(def);
}

// ======== Struct instance ========

//PROBLEM: same thing did second time, almost the same ,but func call

StructInstance* create_struct_instance(JitContext* ctx, const StructDef* def) {
    if (!ctx || !def) return NULL;
    if (!ctx->code_page) {
        if (!jit_alloc_page(ctx)) {
            fprintf(stderr, "[FATAL] Failed to alloc page in struct_instance\n");
            return NULL;
        }
    }

    if (ctx->code_offset + def->total_size > ctx->code_page_size) {
        fprintf(stderr, "[ERROR] Page memory full\n");
        return NULL;
    }

    StructInstance* inst = (StructInstance*)malloc(sizeof(StructInstance));
    if (!inst) return NULL;

    inst->data = (void*)((char*)ctx->code_page + ctx->code_offset);
    inst->def = def;
    ctx->code_offset += def->total_size;
    memset(inst->data, 0, def->total_size);
    return inst;
}

void destroy_struct_instance(StructInstance* inst) {
    if (!inst) return;
    free(inst);
}

//======== Function Def ========
//add function symbol (suppose to be this)
void add_function_field(StructDef* def, const char* name, FunctionHandler default_fn) {
    if (!def || !name) return;

    StructField field;
    field.name = safe_strdup(name);
    field.type = FIELD_FUNCTION;
    field.size = sizeof(void*);
    field.default_handler = (void*)default_fn;

    int last_offset = 0;
    if (def->field_count > 0) {
        StructField* last = &def->fields[def->field_count - 1];
        last_offset = last->offset + last->size;
    }
    field.offset = last_offset;

    def->fields = realloc(def->fields, sizeof(StructField) * (def->field_count + 1));
    def->fields[def->field_count] = field;
    def->field_count++;
}

// renew handler (SEEMS: hot relode)
void update_function_handler(StructField* field, FunctionHandler new_fn) {
    if (field && field->type == FIELD_FUNCTION) {
        field->default_handler = (void*)new_fn;
    }
}

//execute function (ctx can be real data ,or outside context)
void call_function_field(void* instance_data, StructField* field) {
    if (!field || field->type != FIELD_FUNCTION) {
        fprintf(stderr, "[ERR] Invalid function field\n");
        return;
    }

    FunctionHandler fn = *(FunctionHandler*)((char*)instance_data + field->offset);
    if (fn) {
        fn(instance_data);
    } else if (field->default_handler) {
        ((FunctionHandler)(field->default_handler))(instance_data);
    } else {
        fprintf(stderr, "[WARN] No handler bound to field '%s'\n", field->name);
    }
}

// lookfor and call
int call_function_by_name(void* instance_data, const StructDef* def, const char* name) {
    if (!def || !name) return -1;
    for (int i = 0; i < def->field_count; ++i) {
        StructField* f = &def->fields[i];
        if (f->type == FIELD_FUNCTION && strcmp(f->name, name) == 0) {
            call_function_field(instance_data, f);
            return 0;
        }
    }
    fprintf(stderr, "[WARN] function '%s' not found\n", name);
    return -1;
}
//return value is status

//======== Atomic operation ========

// C layer A.O(seems for dispatch)
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

//PROBLEM: WHERE IS BUFFER WRITE ???
//SEEMES: need define oneself

/*
atomic wapper, dont know if use

spinlock_t global_lock = { .value = 0 };
int global_lock_value = 0; // 提供给 main 打印

void atomic_lock_wrapper(void* ctx) {
    atomic_lock(&global_lock);
    global_lock_value = global_lock.value;
    printf("[dispatch] locked\n");
}

void atomic_unlock_wrapper(void* ctx) {
    atomic_unlock(&global_lock);
    global_lock_value = global_lock.value;
    printf("[dispatch] unlocked\n");
}

void atomic_pause_wrapper(void* ctx) {
    atomic_pause();
    printf("[dispatch] pause done\n");
}

*/

//======== panic ========
//PROBLEM: need redo the erroring, and add a debug env and production env for global
//PEOBLEM: need to relocate the error page, maybe move to std/error
// 将错误消息数组移到文件作用域
static const char* const error_messages[] = {
    [0xDEAD0001 - 0xDEAD0001] = "Memory init failed",
    [0xDEAD0002 - 0xDEAD0001] = "Out of memory",
    [0xDEAD0003 - 0xDEAD0001] = "Invalid pointer"
};

void jit_panic(uint32_t code) {
    const char* msg;
    switch(code) {
        case 0xDEAD0001: msg = "内存初始化失败"; break;
        case 0xDEAD0002: msg = "内存耗尽"; break;
        case 0xDEAD0003: msg = "非法指针 (必须为页对齐地址)"; break;
        default: msg = "未知错误";
    }
    fprintf(stderr, "❌ JIT引擎崩溃: %s (代码:0x%X)\n", msg, code);
    fprintf(stderr, "  建议检查: 1.指针是否越界 2.内存是否已初始化\n");
    abort();
}

//logic control and other things need done after analyse

