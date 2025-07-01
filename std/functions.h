#pragma once
#include <stdint.h>
#include "struct_def.h"  // 你已有的结构体定义头

// 通用函数类型：接受结构定义和数据指针，返回int作为状态码
typedef int (*FuncHandler)(StructDef* def, void* data, int argc, void** argv);

// 例：算术函数ID
typedef enum {
    FUNC_ADD = 1,
    FUNC_SUB,
    FUNC_MUL,
    FUNC_DIV,
    FUNC_COMPARE_EQ,
    FUNC_GET_FIELD_INT,
    FUNC_SET_FIELD_INT,
    //these can never be what I want, what I need is data in ram ,read ram and do things ,ram op it self
} FuncID;

// 基础函数声明
int func_add(StructDef* def, void* data, int argc, void** argv);
int func_sub(StructDef* def, void* data, int argc, void** argv);
int func_mul(StructDef* def, void* data, int argc, void** argv);
int func_div(StructDef* def, void* data, int argc, void** argv);

int func_compare_eq(StructDef* def, void* data, int argc, void** argv);

int func_get_field_int(StructDef* def, void* data, int argc, void** argv);
int func_set_field_int(StructDef* def, void* data, int argc, void** argv);

// 函数调用接口（由汇编或C端统一调用）
int call_function(FuncID fid, StructDef* def, void* data, int argc, void** argv);
