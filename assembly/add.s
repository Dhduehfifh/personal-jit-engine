// 文件名: add.s
.globl _my_add
.p2align 2
.text

_my_add:
    // 参数 a → x0，参数 b → x1
    add x0, x0, x1     // x0 = x0 + x1
    ret