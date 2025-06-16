.section __TEXT,__text
.global _dispatch_and_call
.extern _dispatch_table

// void dispatch_and_call(uint8_t opcode)
_dispatch_and_call:
    stp     x29, x30, [sp, #-16]!   // 保存链接寄存器
    mov     x2, x0                  // x0 是 opcode

    adrp    x3, _dispatch_table@PAGE
    add     x3, x3, _dispatch_table@PAGEOFF
    ldr     x4, [x3, x2, lsl #3]    // 取 dispatch_table[x2]
    cbz     x4, .Lend               // 如果是 NULL，跳过
    blr     x4                      // 调用函数指针

.Lend:
    ldp     x29, x30, [sp], #16     // 恢复返回地址
    ret