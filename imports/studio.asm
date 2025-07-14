; studio_dispatch.asm
global studio_dispatch_by_index
section .text
studio_dispatch_by_index:
    ; rdi = ctx
    ; esi = index
    ; 调用 studio_get_by_index() 拿到指针再 call
    ; 可以 link 到 studio_get_by_index via PLT
    push rdi
    call studio_get_by_index
    pop rdi
    call rax
    ret
