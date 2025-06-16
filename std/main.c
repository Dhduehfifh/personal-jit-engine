#include "dispatch_table.h"
#include "memory.h"
#include "dispatch.h"
#include <stdio.h>
#include <pthread.h>

// 准备参数函数：写入冷区（data_page）
void prepare_args(JitContext* ctx, int a, int b) {
    if (!ctx->data_page) {
        printf("[ERROR] Data page not allocated\n");
        return;
    }
    int* base = (int*)ctx->data_page;
    base[0] = a;  // operand1
    base[1] = b;  // operand2
    base[2] = 0;  // result placeholder
}

// 显示结果函数：从冷区读取结果
void print_result(JitContext* ctx, const char* op) {
    int* base = (int*)ctx->data_page;
    printf("[RESULT] %d %s %d = %d\n", base[0], op, base[1], base[2]);
}

// 子线程函数：执行 handler
void* exec_handler(void* arg) {
    uint8_t opcode = *(uint8_t*)arg;
    trigger_handler(opcode);
    return NULL;
}

int main() {
    JitContext ctx;
    jit_init(&ctx);

    // 初始化指令表（0x04 ~ 0x07）
    init_dispatch_table();

    // 注册 alloc/free 动态指令
    register_handler(0x01, jit_alloc_page_handler, &ctx, 1);
    register_handler(0x02, jit_free_page_handler, &ctx, 1);

    // 分配两页（code + data）
    printf("[TEST] Triggering alloc (0x01)\n");
    trigger_handler(0x01);
    printf("[TEST] Alloc done. Code page: %p | Data page: %p\n", ctx.code_page, ctx.data_page);

    // 设置参数
    prepare_args(&ctx, 100, 5);

    // opcode + 符号表
    uint8_t opcodes[] = {0x04, 0x05, 0x06, 0x07};  // add, sub, mul, div
    const char* opsym[] = {"+", "-", "*", "/"};

    for (int i = 0; i < 4; ++i) {
        pthread_t thread;
        printf("[THREAD] Executing opcode 0x%02X (%s)\n", opcodes[i], opsym[i]);

        // 启动线程执行 handler
        pthread_create(&thread, NULL, exec_handler, &opcodes[i]);
        pthread_join(thread, NULL);

        // 打印计算结果
        print_result(&ctx, opsym[i]);
    }

    // 清理
    printf("[TEST] Triggering free (0x02)\n");
    trigger_handler(0x02);
    printf("[TEST] Free done.\n");

    return 0;
}
