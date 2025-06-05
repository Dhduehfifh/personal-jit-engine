// jit_dispatch.S (ARM64)
.globl _dispatch_call

// void dispatch_call(uint8_t page, uint8_t opcode);
_dispatch_call:
    stp x29, x30, [sp, #-16]!
    mov x29, sp

    // page in w0, opcode in w1
    adrp x2, _dispatch_table@PAGE
    add  x2, x2, _dispatch_table@PAGEOFF

    // offset = page * 256 + opcode
    mov  w3, w0
    lsl  w3, w3, #8
    add  w3, w3, w1
    lsl  x3, x3, #3         // *8 (sizeof ptr)

    ldr  x4, [x2, x3]       // 取出函数地址
    cbz  x4, .Lnull_call
    blr  x4
    b    .Ldone

.Lnull_call:
    // 打印错误（通过外部 printf 可加上）
    nop

.Ldone:
    ldp x29, x30, [sp], #16
    ret
