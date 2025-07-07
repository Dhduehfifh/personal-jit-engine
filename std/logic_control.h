// logic_control.h
#pragma once
#include "struct_def.h"
#include "struct_instance.h"

#ifdef __cplusplus
extern "C" {
#endif

// 控制逻辑执行器：检查字段是否等于值，然后触发 handler
// 参数:
//   data - 实例的 data 区指针
//   def  - 结构体定义（包含字段信息）
//   field_name - 需要检查的字段名（例如 "flag"）
//   expected   - 期望值（int）
//   func_field - 对应的函数字段（必须是 FIELD_FUNCTION 类型）
// 返回：是否触发了 handler（1=是，0=否）
int execute_if_equal_then_call(void* data, const StructDef* def,
                               const char* field_name, int expected,
                               const StructField* func_field);

// 汇编实现（参数由 execute_if_equal_then_call 封装）
//   rdi = actual value ptr (int*)
//   esi = expected value (int)
//   rdx = function ptr (void(*)(void*))
//   rcx = context ptr (data)
int logic_branch_eq();  // 汇编实现的函数

#ifdef __cplusplus
}
#endif
