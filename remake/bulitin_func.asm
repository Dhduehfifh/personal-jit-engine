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

compare:
    cmp rdi, rsi
    sete al
    movzx rax, al
    mov     rcx, qword [rel flag_eq]  ; 加载 flag_eq 的地址
    mov     [rel flag_eq], rax        ; 使用 RIP 相对寻址存储
    ret

jump_if_eq:
    mov     rax, [rel flag_eq]
    test    rax, rax
    jz      .skip
    mov     rax, [rdi]
    jmp     rax
.skip:
    ret

jump_if_neq:
    mov     rax, [rel flag_eq]
    test    rax, rax
    jnz     .skip
    mov     rax, [rdi]
    jmp     rax
.skip:
    ret

jump:
    mov     rax, [rdi]
    jmp     rax

add_u64:
    mov     rax, rdi
    add     rax, rsi
    ret

sub_u64:
    mov     rax, rdi
    sub     rax, rsi
    ret

mul_u64:
    mov     rax, rdi
    mul     rsi
    ret

div_u64:
    mov     rax, rdi
    xor     rdx, rdx
    div     rsi
    ret

get_tsc:
    ; rdtsc 指令读取时间戳计数器到 edx:eax，64位值放到 rax 返回
    rdtsc
    shl rdx, 32
    or rax, rdx
    ret