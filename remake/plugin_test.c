#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define OPCODE_SWITCH_LIB  0xF0
#define OPCODE_EXIT_LIB    0xF1

// 模拟配置结构
typedef struct {
    char lib_name[64];
    void (*func_table[4])(void); // 最多 4 个测试函数
} Plugin;

// 测试函数
void plugin_func_a(void) { printf("[Plugin] Func A 执行\n"); }
void plugin_func_b(void) { printf("[Plugin] Func B 执行\n"); }
void plugin_func_c(void) { printf("[Plugin] Func C 执行\n"); }

// 模拟配置加载
Plugin* load_plugin_cfg(const char* cfg_path) {
    printf("[Loader] 读取配置: %s\n", cfg_path);
    Plugin* p = calloc(1, sizeof(Plugin));
    strcpy(p->lib_name, "test_plugin");
    p->func_table[0] = plugin_func_a;
    p->func_table[1] = plugin_func_b;
    p->func_table[2] = plugin_func_c;
    return p;
}

// 主执行函数
void dispatch(uint8_t* buffer, size_t len, Plugin* current_plugin) {
    int in_plugin_mode = 0;
    for (size_t i = 0; i < len; i++) {
        uint8_t op = buffer[i];

        if (op == OPCODE_SWITCH_LIB) {
            in_plugin_mode = 1;
            printf("[VM] 切换到外接库模式: %s\n", current_plugin->lib_name);
            continue;
        }
        if (op == OPCODE_EXIT_LIB) {
            in_plugin_mode = 0;
            printf("[VM] 退出外接库模式\n");
            continue;
        }

        if (in_plugin_mode) {
            // 用纯数学方式映射函数（取模法，防止if else分支）
            size_t index = op % 3;
            if (current_plugin->func_table[index]) {
                current_plugin->func_table[index]();
            }
        } else {
            printf("[VM] 内核执行 opcode: 0x%02X\n", op);
        }
    }
}

int main() {
    // 模拟缓冲区（含切换到外接库模式与退出）
    uint8_t buffer[] = {
        0x01, // 内核指令
        OPCODE_SWITCH_LIB,
        0x00, 0x01, 0x02, // 外接库指令
        OPCODE_EXIT_LIB,
        0x02 // 内核指令
    };

    // 加载插件
    Plugin* p = load_plugin_cfg("plugin.cfg");

    // 执行
    dispatch(buffer, sizeof(buffer), p);

    // 清理
    free(p);
    return 0;
}
