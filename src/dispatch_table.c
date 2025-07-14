#include "dispatch_table.h"
#include "builtin_func.h"
#include <stdio.h>

// 全局dispatch表，初始化为NULL
DispatchHandler dispatch_table[MAX_DISPATCH_OPCODES] = {0};

// 注册handler
void register_handler(uint8_t opcode, DispatchHandler handler) {
    if (opcode == 0 || handler == NULL) return;
    dispatch_table[opcode] = handler;
}

// C调用入口
void dispatch_entry(uint8_t opcode, void* ctx) {
    DispatchHandler fn = dispatch_table[opcode];
    if (fn) {
        fn(ctx);
    } else {
        fprintf(stderr, "[DISPATCH] opcode 0x%02X unbound.\n", opcode);
    }
}

// --- wrapper部分，统一void*传参，调用实际函数 ---

void jit_init_wrapper(void* ctx) {
    jit_init((JitContext*)ctx);
}

void jit_alloc_page_wrapper(void* ctx) {
    // 有返回值但这里忽略，调用时需要另外方式获取
    (void)jit_alloc_page((JitContext*)ctx);
}

void jit_free_wrapper(void* ctx) {
    jit_free((JitContext*)ctx);
}

void* create_struct_def_wrapper(void* ctx) {
    // 传入ctx需要是合适类型，否则这里先只示例占位
    (void)ctx; 
}

void add_field_to_struct_wrapper(void* ctx) {
    (void)ctx;
}

void finalize_struct_wrapper(void* ctx) {
    (void)ctx;
}

void get_field_offset_wrapper(void* ctx) {
    (void)ctx;
}

void destroy_struct_def_wrapper(void* ctx) {
    (void)ctx;
}

void* create_struct_instance_wrapper(void* ctx) {
    (void)ctx;
}

void destroy_struct_instance_wrapper(void* ctx) {
    (void)ctx;
}
/*
void add_function_field_wrapper(void* ctx) {
    (void)ctx;
}

void update_function_handler_wrapper(void* ctx) {
    (void)ctx;
}

void call_function_by_name_wrapper(void* ctx) {
    (void)ctx;
}

void call_function_field_wrapper(void* ctx) {
    (void)ctx;
} */

void atomic_lock_wrapper(void* ctx) {
    atomic_lock((spinlock_t*)ctx);
}

void atomic_unlock_wrapper(void* ctx) {
    atomic_unlock((spinlock_t*)ctx);
}

void atomic_pause_wrapper(void* ctx) {
    atomic_pause();
}

void atomic_compare_and_swap_wrapper(void* ctx) {
    (void)ctx;
}

void jit_panic_wrapper(void* ctx) {
    if (!ctx) {
        jit_panic(0xDEAD0003);  // 非法指针
        return;
    }

    // 简单死区检查：确保指针是8字节对齐（示例，按需调整）
    if ((uintptr_t)ctx & 0x7) {
        jit_panic(0xDEAD0003);
        return;
    }

    uint32_t* code_ptr = (uint32_t*)ctx;
    jit_panic(*code_ptr);
}

void jit_panic_cleanup_wrapper(void* ctx) {
    if (!ctx) {
        return;
    }
    jit_panic_cleanup();
    printf("========END========");
}



// --- 初始化绑定 ---
void noop_wrapper(void* ctx) {
    //empty case
    (void)ctx;
    return;
}

void* noop_return_null_wrapper(void* ctx) {
    (void)ctx;
    return NULL;
}
//reopen one of it ? doesn't suppose to be like that ....

void init_builtin_dispatch() {
    for (int i = 0; i < MAX_DISPATCH_OPCODES; ++i)
        dispatch_table[i] = noop_return_null_wrapper;  // 全部初始化为 safe 函数
    

    register_handler(0x00, noop_return_null_wrapper);
    register_handler(0x01, jit_init_wrapper);
    register_handler(0x02, jit_alloc_page_wrapper);
    register_handler(0x03, jit_free_wrapper);

    register_handler(0x10, create_struct_def_wrapper);
    register_handler(0x11, add_field_to_struct_wrapper);
    register_handler(0x12, finalize_struct_wrapper);
    register_handler(0x13, get_field_offset_wrapper);
    register_handler(0x14, destroy_struct_def_wrapper);

    register_handler(0x15, create_struct_instance_wrapper);
    register_handler(0x16, destroy_struct_instance_wrapper);

    register_handler(0x17, add_function_field_wrapper);
    register_handler(0x18, update_function_handler_wrapper);
    register_handler(0x19, call_function_by_name_wrapper);
    register_handler(0x1A, call_function_field_wrapper);

    register_handler(0x04, atomic_lock_wrapper);
    register_handler(0x05, atomic_unlock_wrapper);
    register_handler(0x06, atomic_pause_wrapper);

    register_handler(0x07, atomic_compare_and_swap_wrapper);

    register_handler(0x1E, jit_panic_wrapper);
    register_handler(0x1F, jit_panic_cleanup_wrapper);

    register_handler(0x30, logic_if_zero_wrapper);
    register_handler(0x31, logic_if_not_zero_wrapper);

    // opcode 0x40~0x45 为 math 运算
    register_handler(0x40, math_add_wrapper);
    register_handler(0x41, math_sub_wrapper);
    register_handler(0x42, math_mul_wrapper);
    register_handler(0x43, math_div_wrapper);
    register_handler(0x44, math_sqrt_wrapper);
    register_handler(0x45, math_pow_wrapper);

}
