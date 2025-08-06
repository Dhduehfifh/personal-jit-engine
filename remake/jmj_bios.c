#include "jmj_bios.h"
#include "dispatch.h"   // init_builtin_table()
#include <stdio.h>

static bool bios_running = false;

int jitvm_bios_boot(void) {
    if (bios_running) {
        printf("[BIOS] Already running.\n");
        return 0;
    }

    printf("[BIOS] Booting JIT VM...\n");

    // 初始化内建指令映射
    init_builtin_table();

    // TODO: 初始化 Terminal / C API / 其他内置软件
    // e.g. terminal_init();
    // e.g. jit_c_api_init();

    bios_running = true;
    printf("[BIOS] Boot complete.\n");
    return 0;
}

void jitvm_bios_shutdown(void) {
    if (!bios_running) {
        printf("[BIOS] Not running.\n");
        return;
    }

    printf("[BIOS] Shutting down JIT VM...\n");

    // TODO: 清理 Terminal / C API / 其他内置软件
    // e.g. terminal_shutdown();
    // e.g. jit_c_api_shutdown();

    bios_running = false;
    printf("[BIOS] Shutdown complete.\n");
}

bool jitvm_bios_is_running(void) {
    return bios_running;
}
