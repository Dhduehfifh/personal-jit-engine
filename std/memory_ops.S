.section __TEXT,__text
.global _asm_buffer_write
.global _asm_buffer_clear

_asm_buffer_write:
    // x0: PageBuffer*, x1: data*, x2: len
    ldr x3, [x0, #16]       // 加载lock地址
    mov w4, #1
1:
    ldaxr w5, [x3]          // 原子加载lock
    cbnz w5, 1b             // 等待解锁
    stxr w6, w4, [x3]       // 尝试加锁
    cbnz w6, 1b             // 失败则重试

    ldr x7, [x0]            // buffer指针
    mov x8, #0
2:                          // 循环写入
    ldrb w9, [x1, x8]
    strb w9, [x7, x8]
    add x8, x8, #1
    cmp x8, x2
    b.lt 2b

    stlr wzr, [x3]          // 释放锁
    ret

_asm_buffer_clear:
    // x0: PageBuffer*
    ldr x1, [x0]            // buffer指针
    ldr x2, [x0, #8]        // capacity
    mov x3, #0
3:                          // 清零循环
    strb wzr, [x1, x3]
    add x3, x3, #1
    cmp x3, x2
    b.lt 3b
    ret