.section __TEXT,__text
.global _call_by_opcode
.extern _dispatch_table

_call_by_opcode:
    // 参数 x0 是 opcode
    // 使用 x1 计算 dispatch_table 的地址（全局变量）

    adrp x1, _dispatch_table@PAGE
    add  x1, x1, _dispatch_table@PAGEOFF

    ldr x2, [x1, x0, lsl #3]    // 每个 entry 是 8 字节函数指针
    cbz x2, _ret
    br x2                      // 跳转执行 handler
_ret:
    ret