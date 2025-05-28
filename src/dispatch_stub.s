.section __TEXT,__text
.global _add_dispatch_jump

// void add_dispatch_jump();
// 可以作为后期汇编优化的接口留空
_add_dispatch_jump:
    ret