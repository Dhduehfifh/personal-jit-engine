section .text
    global dispatch_entry_asm
    extern dispatch_table

; void dispatch_entry_asm(uint8_t opcode, void* ctx)
; 参数:
;   rdi = opcode
;   rsi = ctx

dispatch_entry_asm:
    movzx rcx, dil            ; opcode -> rcx (zero extend)
    mov rax, dispatch_table
    mov rdx, [rax + rcx*8]   ; 取函数指针

    test rdx, rdx
    jz .ret                  ; 如果空指针直接返回

    mov rdi, rsi             ; ctx参数传递
    call rdx                 ; 调用handler

.ret:
    ret
