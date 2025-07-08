section .text
    ;global symbles
    
    global atomic_lock_asm
    global atomic_cas_asm
    global atomic_pause_asm
    global asm_buffer_write



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
