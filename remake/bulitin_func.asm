section .data
    global flag_eq
flag_eq: dq 0

section .text
    global compare
    global jump_if_eq
    global jump_if_neq
    global jump
    global add_u64
    global sub_u64
    global mul_u64
    global div_u64
    global get_tsc
    global plugin_call_bridge

; --------------------------------------
; compare(uint64_t a, uint64_t b)
; 保存比较结果到 flag_eq
; --------------------------------------
compare:
    cmp rdi, rsi
    sete al
    movzx rax, al
    mov [rel flag_eq], rax
    ret

; --------------------------------------
; jump_if_eq(uint64_t* target)
; --------------------------------------
jump_if_eq:
    mov rax, [rel flag_eq]
    test rax, rax
    jz .skip
    mov rax, [rdi]
    jmp rax
.skip:
    ret

; --------------------------------------
; jump_if_neq(uint64_t* target)
; --------------------------------------
jump_if_neq:
    mov rax, [rel flag_eq]
    test rax, rax
    jnz .skip
    mov rax, [rdi]
    jmp rax
.skip:
    ret

; --------------------------------------
; jump(uint64_t* target)
; --------------------------------------
jump:
    mov rax, [rdi]
    jmp rax

; --------------------------------------
; add_u64(uint64_t a, uint64_t b)
; --------------------------------------
add_u64:
    mov rax, rdi
    add rax, rsi
    ret

; --------------------------------------
; sub_u64(uint64_t a, uint64_t b)
; --------------------------------------
sub_u64:
    mov rax, rdi
    sub rax, rsi
    ret

; --------------------------------------
; mul_u64(uint64_t a, uint64_t b)
; --------------------------------------
mul_u64:
    mov rax, rdi
    mul rsi
    ret

; --------------------------------------
; div_u64(uint64_t a, uint64_t b)
; --------------------------------------
div_u64:
    mov rax, rdi
    xor rdx, rdx
    div rsi
    ret

; --------------------------------------
; get_tsc() -> uint64_t
; --------------------------------------
get_tsc:
    rdtsc
    shl rdx, 32
    or  rax, rdx
    ret

; =========================================================
; plugin_call_bridge(func_ptr, args, argc, ret_ptr)
; rdi = func_ptr
; rsi = args
; rdx = argc
; rcx = ret_ptr
; =========================================================
plugin_call_bridge:
    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    ; ---- 参数安全性检查 ----
    ; 禁止 func_ptr 为 NULL
    test rdi, rdi
    jz .invalid

    ; 限制 argc <= 8
    cmp rdx, 8
    jbe .argc_ok
    mov rdx, 8
.argc_ok:

    ; r8 = args 指针
    mov r8, rsi

    ; 循环检查每个参数是否是安全的 uint64_t（禁止高位地址外部指针）
    ; 这里假设 VM 内部地址 < 0x0000800000000000
    mov r9, rdx         ; r9 = argc
    xor r10, r10        ; index = 0
.check_loop:
    cmp r10, r9
    jge .check_done
    mov rax, [r8 + r10*8]
    cmp rax, 0x0000800000000000
    jb  .next_ok
    ; 如果超出范围，直接置零（防止外部指针）
    mov qword [r8 + r10*8], 0
.next_ok:
    inc r10
    jmp .check_loop
.check_done:

    ; ---- 加载寄存器参数 ----
    cmp rdx, 0
    jle .skip_args
    mov rax, [r8]
    mov rdi, rax         ; arg0

    cmp rdx, 1
    jle .skip_args
    mov rax, [r8+8]
    mov rsi, rax         ; arg1

    cmp rdx, 2
    jle .skip_args
    mov rax, [r8+16]
    mov rdx, rax         ; arg2

    cmp rdx, 3
    jle .skip_args
    mov rax, [r8+24]
    mov rcx, rax         ; arg3

    cmp rdx, 4
    jle .skip_args
    mov rax, [r8+32]
    mov r8, rax          ; arg4

    cmp rdx, 5
    jle .skip_args
    mov rax, [r8+40]
    mov r9, rax          ; arg5

.skip_args:

    ; 栈对齐
    mov r11, rsp
    and rsp, -16

    ; 调用
    mov rax, rdi         ; rdi 保存的是 func_ptr
    call rax

    ; 恢复栈
    mov rsp, r11

    ; 保存返回值
    test rcx, rcx
    jz .done
    mov [rcx], rax

.done:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    mov rsp, rbp
    pop rbp
    ret

.invalid:
    ; func_ptr 无效，直接返回 0
    test rcx, rcx
    jz .done
    mov qword [rcx], 0
    jmp .done
