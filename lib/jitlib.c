#include <stdio.h>
#include <stdint.h>

typedef void (*event_fn)();
#define MAX_OPCODES 256

// 汇编会用到这个表
event_fn dispatch_table[MAX_OPCODES];

// 注册函数
void register_handler(uint8_t opcode, event_fn fn) {
    dispatch_table[opcode] = fn;
}

// 示例 handler
void example_event() {
    printf(">> example_event() called!\n");
}

// 获取函数指针（非必须）
event_fn resolve_handler(uint8_t opcode) {
    return dispatch_table[opcode];
}

// 导出给 Python 用
__attribute__((visibility("default"))) void register_handler_export(uint8_t op, event_fn fn) {
    register_handler(op, fn);
}
__attribute__((visibility("default"))) void example_event_export() {
    example_event();
}
__attribute__((visibility("default"))) event_fn resolve_handler_export(uint8_t op) {
    return resolve_handler(op);
}
__attribute__((visibility("default"))) event_fn* dispatch_table_ref() {
    return dispatch_table;
}