; studio.asm
global lib_studio_dispatch_call
extern lib_studio_dispatch_entry

section .text
; void lib_studio_dispatch_call(int code, void* ctx)
lib_studio_dispatch_call:
    ; System V ABI: rdi=code, rsi=ctx

    ; 这里用 RIP 相对调用
    ; call lib_studio_dispatch_entry
    ; jmp lib_studio_dispatch_entry  ; 这个写法容易产生绝对重定位

    ; 推荐写法：
    ; 调用外部符号的跳转，使用 call 之后直接返回
    ; 或者用相对寻址指令 (call or jmp [rel lib_studio_dispatch_entry])

    ; 一种写法是用 call 代替 jmp，参数是 RIP 相对的
    call lib_studio_dispatch_entry
    ret
