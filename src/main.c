#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dispatch_table.h"
#include "builtin_func.h"

// mock panic handler
void my_panic_handler(uint32_t code) {
    printf("[CUSTOM PANIC] Code = 0x%X\n", code);
}

void test_dispatch_system() {
    printf("=== Dispatch Test Start ===\n");

    // 设置 panic handler
    set_jit_panic_handler(my_panic_handler);

    // 初始化 dispatch 表
    init_builtin_dispatch();

    // 分配 JIT 上下文
    JitContext ctx = {
        .code_page = NULL,
        .code_offset = 0,
        .code_page_size = 0,
        .requested_alloc_size = 4096
    };

    // === 1. 初始化内存页 ===
    dispatch_entry(0x01, &ctx); // jit_init
    dispatch_entry(0x02, &ctx); // jit_alloc_page

    // === 2. 测试 atomic 操作 ===
    spinlock_t lock = {0};
    dispatch_entry(0x04, &lock); // atomic_lock
    dispatch_entry(0x06, NULL);  // atomic_pause
    dispatch_entry(0x05, &lock); // atomic_unlock

    // === 3. 创建 StructDef 并添加字段 ===
    StructDef* def = create_struct_def("MyStruct", 0);

    // 使用 wrapper 包装器手动模拟添加字段
    add_field_to_struct(def, "a", FIELD_INT, 0);
    add_field_to_struct(def, "b", FIELD_BOOL, 0);
    add_function_field(def, "run", (FunctionHandler)dispatch_entry); // 绑定假函数

    // finalize struct
    finalize_struct(def);

    // === 4. 查找字段偏移 ===
    int offset = get_field_offset(def, "a");
    printf("[INFO] Field offset of 'a': %d\n", offset);

    // === 5. 创建 struct 实例 ===
    StructInstance* inst = create_struct_instance(&ctx, def);

    // === 6. 调用函数字段 ===
    call_function_by_name(inst->data, def, "run");

    // === 7. 清理资源 ===
    destroy_struct_instance(inst);
    destroy_struct_def(def);
    dispatch_entry(0x03, &ctx); // jit_free

    // === 8. 测试 panic 调用 ===
    dispatch_entry(0x1F, (void*) (uintptr_t)JIT_PANIC_STACK_OVERFLOW);

    printf("=== Dispatch Test End ===\n");
}

int main() {
    test_dispatch_system();
    return 0;
}
