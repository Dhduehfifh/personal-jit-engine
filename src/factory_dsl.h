// factory_dsl.h
#ifndef FACTORY_DSL_H
#define FACTORY_DSL_H

#include <stdint.h>

#ifndef DSL_STACK_SIZE
#define DSL_STACK_SIZE 512  // 单个流水线函数的默认栈大小
#endif

#ifndef DSL_STACK_COUNT
#define DSL_STACK_COUNT 16  // 最大并发流水线函数个数
#endif

#ifdef __cplusplus
extern "C" {
#endif

// DSL 栈结构体
typedef struct {
    uint8_t buffer[DSL_STACK_SIZE];
    uint32_t offset; // 当前写入偏移
    int in_use;      // 是否被占用
} DSLStack;

// 函数指针定义：流水线函数入口
typedef void (*DSLFunc)(DSLStack* stack, void* user_data);

// 函数表结构
typedef struct {
    DSLFunc funcs[256];  // 最多支持 256 个 opcode 映射
    int count;
} DSLFunctionTable;

// 初始化函数池与 stack 区
void dsl_factory_init();

// 分配一个可用 DSL 栈
DSLStack* dsl_alloc_stack();

// 释放 DSL 栈
void dsl_free_stack(DSLStack* stack);

// 注册 DSL 函数
int dsl_register_func(uint8_t opcode, DSLFunc func);

// 执行 DSL 函数
int dsl_execute(uint8_t opcode, void* user_data);

#ifdef __cplusplus
}
#endif

#endif // FACTORY_DSL_H
