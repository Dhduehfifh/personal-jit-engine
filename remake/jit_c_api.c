#include "jit_c_api.h"
#include "jmj_bios.h"
#include "dispatch.h"
#include <stdio.h>
#include <string.h>

static jit_capi_context_t ctx;
static bool capi_running = true;

// ===== 工具箱: 获取全局上下文 =====
jit_capi_context_t* jit_capi_get_context(void) {
    return &ctx;
}

// ===== 生命周期 =====
void jit_capi_boot(void) {
    static arr test_ctx = {0};  // 测试用的数组上下文
    if (!jitvm_bios_is_running()) {
        if (jitvm_bios_boot() != 0) {
            fprintf(stderr, "[CAPI] BIOS boot failed!\n");
            return;
        }
    }

    memset(&ctx, 0, sizeof(ctx));
    capi_running = true;
    printf("[CAPI] Boot complete\n");

    // ===== 在 C 端直接添加 alloc 操作到 opcode 缓冲区 =====
    // 0x01 是 alloc，64 是 size，第三个是 ctx_ptr
    ctx.opcode_buf[ctx.opcode_count++] = 0x01;                  // alloc opcode
    ctx.opcode_buf[ctx.opcode_count++] = 64;                     // size
    ctx.opcode_buf[ctx.opcode_count++] = (uint64_t)&test_ctx;    // ctx_ptr
}


void jit_capi_shutdown(void) {
    if (capi_running) {
        capi_running = false;
        printf("[CAPI] Shutdown\n");
    }
}

// ===== 缓冲区管理 =====
void jit_capi_clear_opcodes(void) {
    ctx.opcode_count = 0;
}

void jit_capi_add_opcode(uint64_t code) {
    if (ctx.opcode_count < JIT_CAPI_MAX_OPCODES) {
        ctx.opcode_buf[ctx.opcode_count++] = code;
    }
}

// ===== 核心执行器 =====
void jit_capi_exec_all(void) {
    if (!capi_running) {
        fprintf(stderr, "[CAPI] Not running! Call jit_capi_boot first.\n");
        return;
    }

    ctx.retval_count = 0;
    for (int i = 0; i < ctx.opcode_count; ) {
    uint64_t code = ctx.opcode_buf[i];
    if (code < BUILTIN_COUNT && builtin_table[code]) {
        if (code == 0x01) { // alloc
            uint64_t size = ctx.opcode_buf[i+1];
            uint64_t ctx_ptr = ctx.opcode_buf[i+2];
            ((void(*)(uint64_t,uint64_t))builtin_table[code])(size, ctx_ptr);
            i += 3;
            continue;
        }
        else {
            builtin_table[code]();
            i++;
            continue;
        }
    }
    else {
        i++;
    }
}

}

// ===== 直接执行外部数组 =====
void jit_capi_exec_array(uint64_t *arr, int len) {
    if (!capi_running) {
        fprintf(stderr, "[CAPI] Not running! Call jit_capi_boot first.\n");
        return;
    }

    ctx.retval_count = 0;
    for (int i = 0; i < len; i++) {
        uint64_t code = arr[i];

        if (code < BUILTIN_COUNT && builtin_table[code]) {
            builtin_table[code]();
        } else {
            // 数据模式
        }
    }
}

// ===== 返回值读取 =====
uint64_t jit_capi_get_retval(int idx) {
    if (idx < ctx.retval_count) {
        return ctx.retval_buf[idx];
    }
    return 0;
}

// 分配整型数据返回值槽
uint64_t jit_capi_ret_alloc_data(uint64_t value) {
    return returns_alloc_data(value);
}

// 分配指针返回值槽
uint64_t jit_capi_ret_alloc_ptr(uint64_t array_id, uint64_t offset, uint64_t size) {
    return returns_alloc_ptr(array_id, offset, size);
}

// 释放返回值槽
void jit_capi_ret_free(uint64_t ret_id) {
    returns_free(ret_id);
}

// 读取返回值槽数据
uint64_t jit_capi_ret_get_data(uint64_t ret_id) {
    uint64_t val = 0;
    if (returns_get_data(ret_id, &val)) {
        return val;
    }
    return 0;
}

// 获取返回值槽指针
ret_entry_t* jit_capi_ret_get_entry(uint64_t ret_id) {
    return returns_get(ret_id);
}
