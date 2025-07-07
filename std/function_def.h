// function_def.h
#pragma once
#include "struct_def.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*FunctionHandler)(void* ctx);

// 注册一个函数字段（类似字段定义）
void add_function_field(StructDef* def, const char* name, FunctionHandler default_fn);

// 替换函数字段的 handler（运行时修改行为）
void update_function_handler(StructField* field, FunctionHandler new_fn);

// 执行函数字段（通过 field 结构）
void call_function_field(void* instance_data, StructField* field);

// 通过名字查找并调用（封装了 offset 计算）
int call_function_by_name(void* instance_data, const StructDef* def, const char* name);

#ifdef __cplusplus
}
#endif
