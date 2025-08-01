#ifndef DISPATCH_H
#define DISPATCH_H

#include <stdint.h>
#include "bulitin_func.h"  // 直接引用你的内建函数声明

// ===== 高位标记 (不与二进制文件冲突) =====
// 用于区分 opcode 与数据
#define OPCODE_BASE 0xFFF0000000000000ull

// ===== 内建函数个数 (0x00 ~ 0x22) =====
#define BUILTIN_COUNT 0x23  // 35 个

// ===== dispatch table 类型 =====
typedef void (*builtin_func_t)(void);

// ===== dispatch table 声明 =====
extern builtin_func_t builtin_table[BUILTIN_COUNT];

// ===== 初始化表 (在 main 之前调用一次) =====
void init_builtin_table(void);

// ===== 执行 opcode =====
// 参数 v 是 uint64_t 数据流中的值
// 会自动去掉 OPCODE_BASE 偏移，并调用对应函数
static inline void dispatch_exec(uint64_t v) {
    uint64_t idx = v - OPCODE_BASE;  // 纯数学运算
    if (idx < BUILTIN_COUNT) {       // 范围判断（防止溢出）
        builtin_table[idx]();
    }
}

#endif
