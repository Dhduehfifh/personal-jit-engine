// main.c
#include <stdio.h>
#include "struct_def.h"
#include "struct_instance.h"
#include "memory.h"

int main() {
    printf("[TEST] Struct Definition & Instance System Start\n");

    // 1. 创建结构定义
    StructDef* point_def = create_struct_def("Point", 0);  // 非 union
    add_field_to_struct(point_def, "x", FIELD_INT, 0);
    add_field_to_struct(point_def, "y", FIELD_INT, 0);
    add_field_to_struct(point_def, "valid", FIELD_BOOL, 0);
    finalize_struct(point_def);

    printf("[INFO] Struct '%s' Total Size = %d bytes\n", point_def->name, point_def->total_size);

    // 2. 创建内存上下文
    JitContext ctx;
    jit_init(&ctx);
    ctx.requested_alloc_size = 4096;

    // 3. 创建结构实例
    StructInstance* point = create_struct_instance(&ctx, point_def);
    if (!point) {
        fprintf(stderr, "[FATAL] Failed to create struct instance\n");
        return 1;
    }

    // 4. 设置字段值
    int* x_ptr = (int*)((char*)point->data + get_field_offset(point_def, "x"));
    int* y_ptr = (int*)((char*)point->data + get_field_offset(point_def, "y"));
    char* valid_ptr = (char*)point->data + get_field_offset(point_def, "valid");

    *x_ptr = 10;
    *y_ptr = 20;
    *valid_ptr = 1;

    // 5. 输出
    printf("[RESULT] Point(x=%d, y=%d, valid=%d)\n", *x_ptr, *y_ptr, *valid_ptr);

    // 6. 清理
    destroy_struct_instance(point);
    destroy_struct_def(point_def);
    jit_free(&ctx);

    return 0;
}
