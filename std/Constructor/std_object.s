// std_object.s

.text
.global _create_object
.global _dispatch_call

_create_object:
    stp     x29, x30, [sp, #-16]!
    mov     x29, sp

    mov     x2, x0
    mov     x3, x1

    mov     x0, #16
    bl      _malloc

    str     w2, [x0]
    str     w3, [x0, #8]

    ldp     x29, x30, [sp], #16
    ret

_dispatch_call:
    adrp    x1, _dispatch_table@PAGE
    add     x1, x1, _dispatch_table@PAGEOFF

    ldr     x2, [x1, x0, lsl #3]
    blr     x2
    ret