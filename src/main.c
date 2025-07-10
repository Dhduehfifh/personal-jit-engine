#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dispatch_table.h"
#include "builtin_func.h"

// 自定义函数用于测试函数字段调用
static void test_function_handler(void* ctx) {
    printf("[FUNCTION FIELD] Called with ctx=%p\n", ctx);
}

// 测试结构体字段访问的辅助函数
static void test_struct_instance(StructInstance* inst) {
    if (!inst || !inst->def || !inst->data) return;

    printf("\n=== Testing Struct Instance ===\n");
    printf("Struct name: %s\n", inst->def->name);
    
    // 模拟写入整数字段
    int* int_field = (int*)((char*)inst->data + get_field_offset(inst->def, "counter"));
    *int_field = 42;
    printf("Set int_field to: %d\n", *int_field);

    // 调用函数字段
    call_function_by_name(inst->data, inst->def, "on_update");
}

int main() {
    float args[2] = {10.0f, 3.0f};
    float* result = (float*)dispatch_table[0x40](args);  // 10 + 3
    printf("10 + 3 = %.2f\n", *result);

}