section .data
msg_add db "[ASM] Add handler called", 10, 0
msg_sub db "[ASM] Sub handler called", 10, 0
msg_mul db "[ASM] Mul handler called", 10, 0
msg_div db "[ASM] Div handler called", 10, 0

section .text
global _op_add
global _op_sub
global _op_mul
global _op_div
extern puts

_op_add:
    mov rdi, msg_add
    call puts
    ret

_op_sub:
    mov rdi, msg_sub
    call puts
    ret

_op_mul:
    mov rdi, msg_mul
    call puts
    ret

_op_div:
    mov rdi, msg_div
    call puts
    ret
