.text
.globl _asm_dispatch              ; ✅ 关键点：正确定义链接器符号名
.no_dead_strip _asm_dispatch     ; ✅ 保证不会被 strip
.align 2

_asm_dispatch:                   ; ✅ 必须加下划线才能正确导出
    lsl x1, x0, #3

    adrp x2, _dispatch_table@GOTPAGE
    ldr x2, [x2, _dispatch_table@GOTPAGEOFF]

    ldr x0, [x2, x1]
    ret

.private_extern _dispatch_table