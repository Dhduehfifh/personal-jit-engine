#include <stdio.h>
#include <stdint.h>
#include "dynlib.h"

extern void* dispatch_table[];

int main() {
    uint8_t instructions[] = { 0x01, 0x02, 0x03 };
    size_t count = sizeof(instructions) / sizeof(instructions[0]);

    printf("=== Mini JIT Runtime ===\n");
    for (size_t i = 0; i < count; ++i) {
        uint8_t opcode = instructions[i];
        printf("[DISPATCH] Opcode 0x%02X\n", opcode);

        void (*handler)(void) = dispatch_table[opcode];
        if (handler) {
            handler();
        } else {
            printf("No handler for opcode 0x%02X\n", opcode);
        }
    }
    return 0;
}