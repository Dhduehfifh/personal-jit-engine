#include <stdint.h>

// 声明 asm 函数
extern void* asm_dispatch(uint8_t opcode);

// 封装为导出符号
__attribute__((visibility("default")))
void* asm_dispatch_export(uint8_t opcode) {
    return asm_dispatch(opcode);
}