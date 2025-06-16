// math_jump.s
.section __TEXT,__text
.global _resolve_jump_address

// void* resolve_jump_address(void** base, uint64_t index)
_resolve_jump_address:
    // base in x0, index in x1
    lsl x1, x1, #3           // x1 = index * 8
    add x0, x0, x1           // x0 = base + index * 8
    ldr x0, [x0]             // x0 = *(base + index)
    ret