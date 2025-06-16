#pragma once
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 分配并返回一个缓冲区指针
void* get_write_ptr();

// 通知接收器写入完成，len 为写入的长度
void commit_write(void* ptr, size_t len);

#ifdef __cplusplus
}
#endif