// jit_core.h
#ifndef JIT_CORE_H
#define JIT_CORE_H

#include <stdint.h>

#define PAGE_MAX  256
#define OPCODE_MAX 256
#define HOT_EXEC_SIZE 4096

typedef void (*jit_func_t)(void);

// 分页指针表
extern jit_func_t dispatch_table[PAGE_MAX][OPCODE_MAX];

// 冷区函数池（静态注册）
extern jit_func_t cold_func_pool[PAGE_MAX][OPCODE_MAX];

// 热区：可执行内存区域
extern uint8_t hot_exec_area[HOT_EXEC_SIZE];

// 函数加载器
void load_func_to_hot(uint8_t page, uint8_t opcode);

// 示例函数
void func_add();
void func_print();

#endif
