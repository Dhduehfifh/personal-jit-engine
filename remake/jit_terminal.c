#include "jit_terminal.h"
#include "jit_c_api.h"
#include "dispatch.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static bool terminal_running = false;
static char input_buf[256];

// ===== 启动终端 =====
void jit_terminal_boot(void) {
    if (!terminal_running) {
        terminal_running = true;
        printf("[Terminal] Boot complete\n");
    }
}

// ===== 停止终端 =====
void jit_terminal_shutdown(void) {
    if (terminal_running) {
        terminal_running = false;
        printf("[Terminal] Shutdown\n");
    }
}

// ===== 状态判断 =====
bool jit_terminal_is_running(void) {
    return terminal_running;
}

// ===== 内部命令处理 =====
static void terminal_handle_command(const char *cmd) {
    if (strcmp(cmd, "quit") == 0) {
        printf("[Terminal] Quit command received\n");
        jit_terminal_shutdown();
    }
    else if (strcmp(cmd, "tree") == 0) {
        printf("[Terminal] Builtin Table:\n");
        for (int i = 0; i < BUILTIN_COUNT; i++) {
            if (builtin_table[i]) {
                printf("  0x%02X : <function>\n", i);
            }
        }
    }
    else if (strncmp(cmd, "exec ", 5) == 0) {
        // 例: exec 0x01 0x02 0x03
        jit_capi_clear_opcodes();

        const char *p = cmd + 5;
        while (*p) {
            while (isspace((unsigned char)*p)) p++;
            if (!*p) break;

            unsigned long long val = 0;
            if (strncmp(p, "0x", 2) == 0) {
                sscanf(p, "%llx", &val);
            } else {
                sscanf(p, "%llu", &val);
            }
            jit_capi_add_opcode((uint64_t)val);

            while (*p && !isspace((unsigned char)*p)) p++;
        }

        jit_capi_exec_all();
    }
    else {
        printf("[Terminal] Unknown command: %s\n", cmd);
    }
}

// ===== 外部输入 =====
void jit_terminal_input(const char *cmd) {
    if (!terminal_running) {
        fprintf(stderr, "[Terminal] Not running! Boot first.\n");
        return;
    }
    strncpy(input_buf, cmd, sizeof(input_buf) - 1);
    input_buf[sizeof(input_buf) - 1] = '\0';
    terminal_handle_command(input_buf);
}

// ===== 同步执行一帧（CLI 模式用） =====
void jit_terminal_tick(void) {
    if (!terminal_running) return;

    printf("jit> ");
    if (!fgets(input_buf, sizeof(input_buf), stdin)) return;

    // 去掉换行
    size_t len = strlen(input_buf);
    if (len > 0 && input_buf[len - 1] == '\n') {
        input_buf[len - 1] = '\0';
    }

    terminal_handle_command(input_buf);
}
