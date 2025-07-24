#ifndef STUDIO_H
#define STUDIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化与清理
int lib_studio_init(void);
void lib_studio_cleanup(void);

// 动态库管理
int lib_studio_switch_library(const char* so_path);
int lib_studio_load_config(const char* config_path);

// 注册函数指针（内部用，一般外部不用调用）
int lib_studio_register_handler(void* fn_ptr, const char* name);

// 通过名称获取函数指针
void* lib_studio_find_handler_by_name(const char* name);

// 由汇编层调用的调度函数
int lib_studio_dispatch_entry(int code, void* ctx);

// 初始化调度表（如果有默认库或默认注册）
void lib_studio_init_builtin_dispatch(void);

#ifdef __cplusplus
}
#endif

#endif // STUDIO_H
