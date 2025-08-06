#include "returns.h"
#include <string.h>
#include <limits.h>

// =========================
// 内部函数：分配空闲槽
// =========================
static uint64_t alloc_slot(return_pool_t* pool) {
    if (!pool) return UINT64_MAX;
    for (uint64_t i = 0; i < RETURN_POOL_SIZE; i++) {
        if (!pool->entries[i].used) {
            pool->entries[i].used = true;
            return i;
        }
    }
    return UINT64_MAX; // 没有空闲槽
}

// =========================
// 初始化返回池
// =========================
void return_pool_init(return_pool_t* pool) {
    if (!pool) return;
    memset(pool, 0, sizeof(return_pool_t));
}

// =========================
// 分配普通数据类型
// =========================
uint64_t return_pool_alloc_data(return_pool_t* pool, uint64_t value) {
    if (!pool) return UINT64_MAX;

    uint64_t id = alloc_slot(pool);
    if (id == UINT64_MAX) return id;

    return_entry_t* e = &pool->entries[id];
    e->type = RET_TYPE_DATA;
    e->array_id = 0;
    e->offset = 0;
    e->size = 1;
    e->buf[0] = value;

    return id;
}

// =========================
// 分配指针类型
// =========================
uint64_t return_pool_alloc_ptr(return_pool_t* pool, uint64_t array_id, uint64_t offset, uint64_t size) {
    if (!pool) return UINT64_MAX;

    uint64_t id = alloc_slot(pool);
    if (id == UINT64_MAX) return id;

    return_entry_t* e = &pool->entries[id];
    e->type = RET_TYPE_PTR;
    e->array_id = array_id;
    e->offset = offset;
    e->size = size;

    return id;
}

// =========================
// 获取普通数据
// =========================
bool return_pool_get_data(const return_pool_t* pool, uint64_t id, uint64_t* out_value) {
    if (!pool) return false;
    if (id >= RETURN_POOL_SIZE) return false;

    const return_entry_t* e = &pool->entries[id];
    if (!e->used || e->type != RET_TYPE_DATA) return false;

    if (out_value) *out_value = e->buf[0];
    return true;
}

// =========================
// 获取条目指针
// =========================
return_entry_t* return_pool_get(return_pool_t* pool, uint64_t id) {
    if (!pool) return NULL;
    if (id >= RETURN_POOL_SIZE) return NULL;

    return_entry_t* e = &pool->entries[id];
    if (!e->used) return NULL;

    return e;
}

// =========================
// 释放一个返回值
// =========================
void return_pool_free(return_pool_t* pool, uint64_t id) {
    if (!pool) return;
    if (id >= RETURN_POOL_SIZE) return;

    pool->entries[id].used = false;
}
//