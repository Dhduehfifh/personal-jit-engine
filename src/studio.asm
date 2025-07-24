global lib_studio_dispatch_call
extern lib_studio_dispatch_entry

section .text

; void lib_studio_dispatch_call(int code, void* ctx)
lib_studio_dispatch_call:
    ; 参数根据 System V ABI: rdi=code, rsi=ctx
    ; 直接调用 C 层的 dispatch_entry
    jmp lib_studio_dispatch_entry
