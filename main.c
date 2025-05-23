#include <stdint.h>
#include <stdio.h>

extern void parse_and_print(const char* input);
extern void output_sys_int(uintptr_t val);  // 测试输出地址用

int main() {
    const char* test = "start{MOV R1, #5}middle{ADD R2, R1, #3}end";
    parse_and_print(test);                 // 应输出 2 次解析内容
    output_sys_int((uintptr_t)test);       // 输出地址用于调试
    return 0;
}