// dispatch.S (ARM64)
.section __TEXT,__text
.global _jit_dispatch

// dispatch.S
_jit_dispatch:
    // x0=opcode, x1=JitContext*
    adrp x16, _dispatch_table@PAGE
    add x16, x16, _dispatch_table@PAGEOFF
    ldr x17, [x16, x0, lsl #3]
    mov x0, x1        // 将上下文指针作为第一个参数
    br x17