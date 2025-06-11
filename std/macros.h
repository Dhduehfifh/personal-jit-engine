#pragma once

#define PAGE_SIZE 4096
#define UNUSED(x) (void)(x)
#define ASM_SYMBOL(x) asm(#x)

// 内存屏障宏
#define MEMORY_BARRIER() __asm__ __volatile__("dmb ish" ::: "memory")