#include "jmj_engine.h"
#include "builtin_func.h"
#include "dispatch_table.h"

#include <string.h>
#include <stdio.h>

#define MAX_BYTECODE_SIZE 4096

static JitContext ctx;
static uint8_t bytecode_buffer[MAX_BYTECODE_SIZE];
static size_t bytecode_len = 0;

static int is_initialized = 0;
static jmj_log_cb log_cb = NULL;
static jmj_panic_cb panic_cb = NULL;

// ===== 初始化 VM 引擎 =====
int jmj_init(size_t page_size) {
    if (is_initialized) return 0;
    memset(&ctx, 0, sizeof(JitContext));
    ctx.requested_alloc_size = page_size;
    jit_init(&ctx);
    jit_alloc_page(&ctx);
    init_builtin_dispatch();
    is_initialized = 1;
    return 1;
}

void jmj_shutdown() {
    if (!is_initialized) return;
    jit_free(&ctx);
    is_initialized = 0;
}

// ===== 外部 callback 注册 =====
void jmj_set_log_callback(jmj_log_cb fn) { log_cb = fn; }
void jmj_set_panic_callback(jmj_panic_cb fn) { panic_cb = fn; }

// ===== 指令推送：单条 =====
int jmj_push_opcode(uint8_t opcode) {
    if (!is_initialized) return -1;
    if (dispatch_table[opcode]) {
        dispatch_entry(opcode, &ctx);  // 命中函数区域
    } else {
        // 写入 struct 区域 → 模拟数据通道写入
        if (bytecode_len < MAX_BYTECODE_SIZE)
            bytecode_buffer[bytecode_len++] = opcode;
    }
    return 0;
}

// ===== 字节码批量推送 =====
int jmj_push_bytes(const uint8_t* buf, size_t len) {
    if (!is_initialized) return -1;
    for (size_t i = 0; i < len; ++i) {
        jmj_push_opcode(buf[i]);
    }
    return 0;
}

// ===== 手动执行 VM（可选）=====
int jmj_execute() {
    for (size_t i = 0; i < bytecode_len; ++i) {
        uint8_t op = bytecode_buffer[i];
        if (dispatch_table[op]) {
            dispatch_entry(op, &ctx);
        }
        // 非 opcode 跳过或处理为数据操作，可扩展
    }
    bytecode_len = 0; // 执行完清空
    return 0;
}

// ===== 获取执行结果 =====
jmj_result_t jmj_get_result() {
    jmj_result_t res = {
        .data = ctx.code_page,
        .size = ctx.code_offset
    };
    return res;
}
