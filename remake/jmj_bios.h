#ifndef JMJ_BIOS_H
#define JMJ_BIOS_H

#include <stdint.h>
#include <stdbool.h>

// BIOS 启动
// 初始化 VM 环境（dispatch table, return page table 等）
// 启动所有内置软件（Terminal, C API...）
// 返回 0 表示成功
int jitvm_bios_boot(void);

// BIOS 关机
// 释放所有资源
void jitvm_bios_shutdown(void);

// BIOS 状态
bool jitvm_bios_is_running(void);

#endif // JMJ_BIOS_H
