#include "receiver.h"
#include <stdio.h>

// 汇编函数声明
extern void write_some_data();

int main() {
    printf("== JIT-like Instruction Writer ==\n");
    write_some_data();  // 调用汇编逻辑
    return 0;
}