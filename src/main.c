#include <stdio.h>
#include <stdint.h>
#include "dispatch_table.h"

// 汇编函数声明
extern void dispatch_and_call(uint8_t opcode);

int main() {
    uint8_t instructions[] = { 0x01, 0x02, 0x03 };
    size_t count = sizeof(instructions) / sizeof(instructions[0]);

    printf("=== Mini JIT Engine ===\n");
    for (size_t i = 0; i < count; ++i) {
        uint8_t opcode = instructions[i];
        printf("[DISPATCH] Opcode 0x%02X\n", opcode);
        dispatch_and_call(opcode);
    }

    return 0;
}