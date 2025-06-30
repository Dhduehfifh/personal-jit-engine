; object_entry.asm
global create_object_entry
extern create_object_from_mod

section .text
create_object_entry:
    ; rdi = ctx
    ; rsi = mod
    ; 调用 C 函数
    push rbp
    mov rbp, rsp

    call create_object_from_mod

    mov rsp, rbp
    pop rbp
    ret
