; logic_control.asm
; 构建简单的逻辑判断 + 调用流程
; 判断 *(rdi) == esi ，若相等则调用 rdx(ctx)
; rdi: 实际值地址（int*）
; esi: 预期值
; rdx: 函数指针 (void (*)(void*))
; rcx: context 参数

global logic_branch_eq
section .text

logic_branch_eq:
    ; 加载 *(rdi)
    mov eax, [rdi]

    ; 比较实际值和预期值
    cmp eax, esi
    jne .no_call     ; 若不相等，跳转

    ; 相等时：调用函数指针 rdx
    mov rdi, rcx     ; 第一个参数设置为 context（data）
    call rdx

    mov eax, 1       ; 返回值：1 表示执行了 handler
    ret

.no_call:
    xor eax, eax     ; 返回 0，未调用
    ret
