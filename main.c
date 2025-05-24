#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_BUFFER 4096

char instruction_buffer[MAX_BUFFER];
size_t buffer_index = 0;

// 🧩 数学分支处理（假设最多 16 个虚拟指令）
void process_command(const char* instr) {
    uint64_t hash = 0;

    // 使用一个简单的 hash 函数替代 if 判断
    for (size_t i = 0; instr[i] != '\0'; ++i) {
        hash = (hash * 31 + instr[i]) % 997;
    }

    // 数学方式路由：用 hash 映射函数指针表
    uint64_t route = hash % 4;

    switch (route) {
        case 0: printf("[执行] MOV 指令 -> %s\n", instr); break;
        case 1: printf("[执行] ADD 指令 -> %s\n", instr); break;
        case 2: printf("[执行] LOAD 指令 -> %s\n", instr); break;
        case 3: printf("[执行] STORE 指令 -> %s\n", instr); break;
        default: printf("[执行] 未知 -> %s\n", instr); break;
    }
}

// 🧷 向缓存写入一段指令
void add_instruction(const char* str) {
    size_t len = strlen(str);
    if (buffer_index + len + 1 >= MAX_BUFFER) return;

    memcpy(&instruction_buffer[buffer_index], str, len);
    buffer_index += len;
    instruction_buffer[buffer_index++] = '\n';
}

// 📦 执行整个暂存区中的所有指令（以 { ... } 为单位）
void run_instruction_buffer() {
    char temp[256];
    int recording = 0;
    int temp_idx = 0;

    for (size_t i = 0; i < buffer_index; ++i) {
        char c = instruction_buffer[i];

        if (c == '{') {
            recording = 1;
            temp_idx = 0;
        } else if (c == '}') {
            temp[temp_idx] = '\0';
            process_command(temp);
            recording = 0;
        } else if (recording && temp_idx < 255) {
            temp[temp_idx++] = c;
        }
    }
}

int main() {
    // ✅ 模拟传入指令
    add_instruction("{MOV R1, #5}");
    add_instruction("{ADD R2, R1}");
    add_instruction("{LOAD R3, [R2]}");
    add_instruction("{STORE R3, [0x100]}");

    // 🚀 解析并执行
    run_instruction_buffer();
    return 0;
}