#ifndef JMJ_JIT_TERMINAL_H
#define JMJ_JIT_TERMINAL_H

#include <stdint.h>
#include <stdbool.h>

// 启动终端（由 BIOS 调用）
void jit_terminal_boot(void);

// 停止终端
void jit_terminal_shutdown(void);

// 判断终端是否运行
bool jit_terminal_is_running(void);

// 终端输入（来自 Python / 外部 API）
void jit_terminal_input(const char *cmd);

// 终端执行一次循环（如果是异步可不调用）
void jit_terminal_tick(void);

#endif // JMJ_JIT_TERMINAL_H
