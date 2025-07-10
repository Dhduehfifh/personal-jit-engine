section .text
    ;global symbles
    ;======== atomic operation ========
    global atomic_lock_asm
    global atomic_unlock_asm
    global atomic_cas_asm
    global atomic_pause_asm
    global asm_buffer_write

    ;======== logic control ========
    global logic_if_zero_wrapper
    global logic_if_not_zero_wrapper

    ;======== math ========
    global math_add_asm
    global math_sub_asm
    global math_mul_asm
    global math_div_asm
    global math_mod_asm



;symble define
; int atomic_lock_asm(int* lock)
atomic_lock_asm:
    mov eax, 0
    mov ecx, 1
    lock cmpxchg dword [rdi], ecx
    sete al
    ret

; void atomic_unlock_asm(int* lock)
atomic_unlock_asm:
    mov dword [rdi], 0
    ret

; int atomic_cas_asm(int* addr, int expected, int desired)
atomic_cas_asm:
    mov eax, esi
    lock cmpxchg dword [rdi], edx
    sete al
    ret

; void atomic_pause_asm()
atomic_pause_asm:
    pause
    ret


asm_buffer_write:
    ret
    ;看来我这个需要内嵌一个跨平台汇编模拟器了，而且还得是那种全原子操作的那种.....
    ;I didnt extern this symble in builtin_func.c/.h, it not gonna be ues this time


; void* logic_if_zero_wrapper(void* ctx)
; rdi = ctx (StructInstance* or other struct)
logic_if_zero_wrapper:
    mov rax, [rdi]           ; 假设 rdi 指向 struct → struct 中前 8 字节为待检查 int 值
    cmp rax, 0
    jne .not_zero

    ; 为 true 分支：我们将 ctx + 8 的位置写 1（假设那里是标志位）
    mov qword [rdi + 8], 1

.not_zero:
    xor rax, rax             ; 返回 NULL
    ret

; void* logic_if_not_zero_wrapper(void* ctx)
logic_if_not_zero_wrapper:
    mov rax, [rdi]
    cmp rax, 0
    je .zero

    mov qword [rdi + 8], 1

.zero:
    xor rax, rax
    ret

;======== Math functions ========
math_add_asm:
    mov eax, edi        ; arg1 in edi
    add eax, esi        ; arg2 in esi
    ret

math_sub_asm:
    mov eax, edi
    sub eax, esi
    ret

math_mul_asm:
    mov eax, edi
    imul eax, esi
    ret

math_div_asm:
    ; dividend = edi, divisor = esi
    mov eax, edi
    cdq                 ; sign extend into edx
    idiv esi
    ret

math_mod_asm:
    mov eax, edi
    cdq
    idiv esi
    mov eax, edx        ; remainder
    ret

    