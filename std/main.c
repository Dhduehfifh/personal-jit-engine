// main.c
#include "dispatch.h"
#include "memory.h"
#include <stdio.h>

// 寄存器上下文结构（根据ARM64 ABI）
typedef struct {
    uint64_t x[29];  // 通用寄存器x0-x28
    uint64_t fp;     // 帧指针x29
    uint64_t lr;     // 链接寄存器x30
    uint64_t sp;     // 栈指针x31
} JitContext;

// 模拟指令执行后的寄存器状态
static JitContext cpu_context;

int main() {
    init_memory_system();
    init_dispatch_table();

    // 测试分配→释放流程
    uint8_t op_alloc = 0x01;
    uint8_t op_free = 0x02;

    // 1. 执行分配指令（0x01）
    cpu_context.x[0] = op_alloc;  // opcode通过x0传递
    jit_dispatch(op_alloc);
    
    // 获取返回值（ARM64下x0存放返回值）
    void* allocated_page = (void*)cpu_context.x[0];
    printf("Allocated page at: %p\n", allocated_page);

    // 2. 执行释放指令（0x02）
    cpu_context.x[0] = op_free;   // opcode
    cpu_context.x[1] = (uint64_t)allocated_page;  // 参数1：要释放的地址
    jit_dispatch(op_free);
    printf("Page freed\n");

    return 0;
}