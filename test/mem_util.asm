; mem_util.asm
section .text
    global align_ptr
    global add_offset
    global call_libc_puts

extern puts

; 内存对齐：向上对齐到 8 字节
align_ptr:
    ; 输入: RDI = 原始指针
    ; 输出: RAX = 对齐后指针
    mov     rax, rdi
    add     rax, 7
    and     rax, -8
    ret

; 指针偏移：
add_offset:
    ; RDI = base, RSI = offset
    mov     rax, rdi
    add     rax, rsi
    ret

; 调用 libc 的 puts()
call_libc_puts:
    ; RDI = const char* str
    call    puts
    ret
