#ifndef RETURNS_H
#define RETURNS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h> // size_t

// =========================
// 返回值类型
// =========================
typedef enum {
    RET_TYPE_DATA = 0, // 普通数据（直接存 buf[]）
    RET_TYPE_PTR  = 1  // 指针（指向 VM 内部 array 内存）
} return_type_t;

// =========================
// 返回值条目
// =========================
typedef struct {
    bool used;                // 是否已占用
    return_type_t type;       // 数据类型
    uint64_t array_id;        // RET_TYPE_PTR: VM 内部 array ID
    uint64_t offset;          // RET_TYPE_PTR: 在 array 中的偏移
    uint64_t size;            // 数据大小（单位 uint64_t）
    uint64_t buf[4];          // RET_TYPE_DATA: 存储区（最多 4 个 uint64_t，可扩展）
} return_entry_t;

// =========================
// 返回值池（每个 VM / 线程独立）
// =========================
#define RETURN_POOL_SIZE 256

typedef struct {
    return_entry_t entries[RETURN_POOL_SIZE];
} return_pool_t;

// =========================
// API（实例化的 return_pool_t 操作）
// =========================

/**
 * 初始化返回池
 */
void return_pool_init(return_pool_t* pool);

/**
 * 分配一个普通数据类型的返回值
 */
uint64_t return_pool_alloc_data(return_pool_t* pool, uint64_t value);

/**
 * 分配一个指针类型的返回值
 */
uint64_t return_pool_alloc_ptr(return_pool_t* pool, uint64_t array_id, uint64_t offset, uint64_t size);

/**
 * 获取普通数据
 */
bool return_pool_get_data(const return_pool_t* pool, uint64_t id, uint64_t* out_value);

/**
 * 获取返回值条目指针
 */
return_entry_t* return_pool_get(return_pool_t* pool, uint64_t id);

/**
 * 释放一个返回值
 */
void return_pool_free(return_pool_t* pool, uint64_t id);

#endif // RETURNS_H
