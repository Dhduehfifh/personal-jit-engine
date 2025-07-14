#ifndef STUDIO_H
#define STUDIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化（无参）和清理
int studio_init(void);
void studio_cleanup(void);

// 动态切换挂载库，传入库路径，返回0成功
int studio_switch_library(const char* so_path);

// 通过函数名调用动态库函数，传入参数指针（可为NULL）
int studio_call_function(const char* func_name, void* arg);

#ifdef __cplusplus
}
#endif

#endif // STUDIO_H
