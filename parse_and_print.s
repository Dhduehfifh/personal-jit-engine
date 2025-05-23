.section __TEXT,__text
.global _parse_and_print
_parse_and_print:
    mov x1, x0              // x1 = input ptr
    adrp x3, buffer@PAGE
    add  x3, x3, buffer@PAGEOFF
    mov x4, #0              // buffer index
    mov x2, #0              // capture_flag

.loop:
    ldrb w5, [x1], #1       // w5 = *x1++; 读取1字节字符
    cbz w5, .end            // if null -> end

    cmp w5, #'{'            // 检测开始符
    b.eq .check_open
    cmp w5, #'}'            // 检测结束符
    b.eq .check_close

    cmp x2, #1              // capture_flag = 1?
    b.ne .loop

    cmp x4, #254            // buffer最多254字节+1终止符
    b.ge .loop
    strb w5, [x3, x4]       // 写入字符
    add x4, x4, #1
    b .loop

.check_open:
    mov x2, #1              // 开始捕获
    mov x4, #0              // 重置 index
    b .loop

.check_close:
    mov x2, #0
    strb wzr, [x3, x4]      // ✅ 写入 null terminator
    mov x0, x3
    bl _output_sys
    mov x4, #0
    b .loop

.end:
    ret

.section __DATA,__data
buffer: .space 256