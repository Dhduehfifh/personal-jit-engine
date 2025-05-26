.text
.global _asm_ifelse
.global _asm_math

// int asm_ifelse(int x)
_asm_ifelse:
    cmp w0, #42
    b.eq .matched
    mov w0, #0
    ret
.matched:
    mov w0, #1
    ret

// int asm_math(int x)
_asm_math:
    // 计算：(x == 42) -> 返回 1，否则 0
    mov w1, #42
    sub w2, w0, w1         // x - 42
    cmp w2, #0
    cset w0, eq            // if x == 42, w0 = 1
    ret