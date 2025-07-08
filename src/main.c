#include <stdio.h>
#include <stdint.h>
#include "dispatch_table.h"

// 外部汇编函数
extern void call_by_opcode(uint8_t opcode);

int main() {
    uint8_t instructions[] = {0x01, 0x02, 0x03};
    size_t count = sizeof(instructions) / sizeof(instructions[0]);

    printf("=== Actinium JIT Runtime ===\n");
    for (size_t i = 0; i < count; ++i) {
        printf("Dispatching opcode 0x%02X...\n", instructions[i]);
        call_by_opcode(instructions[i]);
    }
    return 0;
}