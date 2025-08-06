#ifndef JMJ_JIT_C_API_H
#define JMJ_JIT_C_API_H

#include <stdint.h>
#include <stdbool.h>
#include "returns.h"

#define JIT_CAPI_MAX_OPCODES 1024
#define JIT_CAPI_MAX_RETVALS 64

// ===== 缓冲区结构 =====
typedef struct {
    uint64_t opcode_buf[JIT_CAPI_MAX_OPCODES]; // opcode + data 混合流
    uint64_t retval_buf[JIT_CAPI_MAX_RETVALS]; // 返回值缓冲区
    int opcode_count; // 当前缓冲区中已写入的数量
    int retval_count; // 当前返回值个数
} jit_capi_context_t;

// ===== BIOS / API 生命周期 =====

//need extern
#ifdef __cplusplus
extern "C" {
#endif
void jit_capi_boot(void);


void jit_capi_shutdown(void);

// ===== 缓冲区管理 =====
void jit_capi_clear_opcodes(void);
void jit_capi_add_opcode(uint64_t code);
void jit_capi_exec_all(void);

// ===== 直接执行外部 arr* =====
void jit_capi_exec_array(uint64_t *arr, int len);

// ===== 工具箱 API =====
jit_capi_context_t* jit_capi_get_context(void);
uint64_t jit_capi_get_retval(int idx);

// ===== 返回值池操作 =====

// 分配返回值槽，返回槽 id（用于后续查询）
uint64_t jit_capi_ret_alloc_data(uint64_t value);
uint64_t jit_capi_ret_alloc_ptr(uint64_t array_id, uint64_t offset, uint64_t size);

// 释放返回值槽
void jit_capi_ret_free(uint64_t ret_id);

// 读取返回值槽内容（简单示例只返回第一个 uint64_t）
uint64_t jit_capi_ret_get_data(uint64_t ret_id);

// 读取返回值槽结构（返回指针，注意内存管理）
ret_entry_t* jit_capi_ret_get_entry(uint64_t ret_id);

#ifdef __cplusplus
}
#endif


#endif // JMJ_JIT_C_API_H
