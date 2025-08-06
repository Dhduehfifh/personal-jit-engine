#ifndef DISPATCH_H
#define DISPATCH_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
#define BUFFER_MAX 1024
#define MAX_ARGS   8
#define CONST_NUMBER (UINT64_MAX - 25) // opcode 映射规则

// 统一的函数指针类型（8 个 uint64_t 参数）
typedef void (*dispatch_func_t)(uint64_t, uint64_t, uint64_t, uint64_t,
                                 uint64_t, uint64_t, uint64_t, uint64_t);

// 初始化分发表
void dispatch_init(void);

// 注册一个函数到指定 opcode
void dispatch_register(uint64_t opcode, dispatch_func_t func);

// 自动注册 bulitin_func.h 里的所有函数
void dispatch_register_all_builtin(void);

// 执行程序
void dispatch_execute(uint64_t *program, uint64_t start_ptr, uint64_t end_ptr);

#ifdef __cplusplus
}
#endif

#endif // DISPATCH_H
