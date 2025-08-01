; plugin_call_bridge.asm (调试版)
; void plugin_call_bridge(void *func_ptr, void **args, int argc, uint64_t *ret_ptr)
; System V AMD64 ABI

global plugin_call_bridge
extern printf

section .rodata
fmt_call:      db "[DEBUG] func_ptr=%p argc=%d ret_ptr=%p", 10, 0
fmt_arg:       db "[DEBUG]   arg[%d] = %p", 10, 0
fmt_ret:       db "[DEBUG] return value=%p", 10, 0
fmt_nullfunc:  db "[DEBUG] func_ptr is NULL!", 10, 0

section .text
plugin_call_bridge:
    ; rdi = func_ptr
    ; rsi = args
    ; rdx = argc
    ; rcx = ret_ptr

    push rbp
    mov rbp, rsp
    push rbx
    push r12
    push r13
    push r14
    push r15

    ; ---- 打印函数信息 ----
    mov r8, rcx                ; 保存 ret_ptr
    mov rcx, r8                ; printf 第 3 参数
    mov r8, rdx                ; printf 第 2 参数
    mov rdx, r8
    mov rsi, rdx                ; printf 第 2 参数
    mov rdx, rsi
    mov rsi, rdx                ; 占位
    mov rsi, rdx
    mov rsi, rdx

    ; 直接用寄存器传递
    mov rsi, rdx
    mov rsi, rdx

    ; 打印基本信息
    mov rsi, rdx
    mov rdx, r8                 ; argc
    mov rsi, rdi                ; func_ptr
    mov rdi, fmt_call
    mov rax, 0
    call printf

    ; ---- 检查 func_ptr 是否 NULL ----
    test rdi, rdi
    jnz .not_null
    mov rdi, fmt_nullfunc
    mov rax, 0
    call printf
    jmp .return_null

.not_null:
    ; 限制 argc <= 8
    cmp rdx, 8
    jbe .argc_ok
    mov rdx, 8
.argc_ok:

    ; ---- 打印每个参数 ----
    mov rbx, rsi        ; rsi=args
    mov r12, rdx        ; argc
    xor r13, r13        ; i = 0
.print_loop:
    cmp r13, r12
    jge .print_done
    mov rax, [rbx + r13*8]
    mov rdi, fmt_arg
    mov rsi, r13
    mov rdx, rax
    mov rax, 0
    call printf
    inc r13
    jmp .print_loop
.print_done:

    ; ---- 加载寄存器参数 ----
    mov r8, rsi          ; args
    cmp r12, 0
    jle .skip_args
    mov rax, [r8]
    mov rdi, rax         ; arg0

    cmp r12, 1
    jle .skip_args
    mov rax, [r8+8]
    mov rsi, rax         ; arg1

    cmp r12, 2
    jle .skip_args
    mov rax, [r8+16]
    mov rdx, rax         ; arg2

    cmp r12, 3
    jle .skip_args
    mov rax, [r8+24]
    mov rcx, rax         ; arg3

    cmp r12, 4
    jle .skip_args
    mov rax, [r8+32]
    mov r8, rax          ; arg4

    cmp r12, 5
    jle .skip_args
    mov rax, [r8+40]
    mov r9, rax          ; arg5

.skip_args:

    ; ---- 栈对齐 ----
    mov r11, rsp
    and rsp, -16

    ; 调用函数
    mov rax, rdi         ; func_ptr 在第一个参数之前传进来
    call rax

    ; 恢复栈
    mov rsp, r11

    ; 保存返回值
    test rcx, rcx
    jz .done
    mov [rcx], rax

    ; 打印返回值
    mov rdi, fmt_ret
    mov rsi, rax
    mov rax, 0
    call printf

    jmp .done

.return_null:
    test rcx, rcx
    jz .done
    xor rax, rax
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


;curbrk error, seems not the problem of libc.so.6