#include <stdio.h>
#include "function_def.h"
#include "struct_def.h"
#include "memory.h"
#include "struct_instance.h"
#include "jit_panic.h"

void hello_fn(void* ctx) {
    printf("[Function] Hello from struct function!\n");
}

void bye_fn(void* ctx) {
    printf("[Function] Goodbye from struct function!\n");
}

int main() {
    printf("== Test Function Field in Struct ==\n");

    // 1. 创建结构定义
    StructDef* def = create_struct_def("TestFn", 0);
    if (!def) {
        fprintf(stderr, "[FATAL] StructDef creation failed\n");
        return 1;
    }

    add_function_field(def, "say_hello", hello_fn);
    finalize_struct(def);

    // 2. 初始化 JIT 内存上下文
    JitContext ctx;
    jit_init(&ctx);
    ctx.requested_alloc_size = 4096;

    // 3. 创建结构实例
    StructInstance* inst = create_struct_instance(&ctx, def);
    if (!inst) {
        fprintf(stderr, "[FATAL] Failed to create struct instance\n");
        destroy_struct_def(def);
        return 1;
    }

    // 4. 查找字段并验证 default handler 是否存在
    StructField* f = &def->fields[0];
    if (!f || f->type != FIELD_FUNCTION) {
        fprintf(stderr, "[ERROR] Field is missing or not a function\n");
        destroy_struct_instance(inst);
        destroy_struct_def(def);
        jit_free(&ctx);
        return 1;
    }

    if (!f->default_handler) {
        fprintf(stderr, "[ERROR] Default handler is NULL\n");
        destroy_struct_instance(inst);
        destroy_struct_def(def);
        jit_free(&ctx);
        return 1;
    }

    // 5. 执行默认 handler
    call_function_field(inst->data, f);

    // 6. 替换 handler 并执行
    update_function_handler(f, bye_fn);
    call_function_field(inst->data, f);

    // 7. 清理
    /*destroy_struct_instance(inst);
    destroy_struct_def(def);
    jit_free(&ctx);*/

    return 0;
}