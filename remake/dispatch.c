#include "dispatch.h"

// ===== dispatch table 定义 =====
builtin_func_t builtin_table[BUILTIN_COUNT];

// ===== 初始化表 =====
void init_builtin_table(void) {
    // 对应 0x00 ~ 0x22 的函数指针顺序排布
    builtin_table[0x00] = (builtin_func_t)empty;
    builtin_table[0x01] = (builtin_func_t)alloc;
    builtin_table[0x02] = (builtin_func_t)release;
    builtin_table[0x03] = (builtin_func_t)termiate;
    builtin_table[0x04] = (builtin_func_t)compare;
    builtin_table[0x05] = (builtin_func_t)jump_if_eq;
    builtin_table[0x06] = (builtin_func_t)jump_if_neq;
    builtin_table[0x07] = (builtin_func_t)jump;
    builtin_table[0x08] = (builtin_func_t)start_timer;
    builtin_table[0x09] = (builtin_func_t)end_timer;
    builtin_table[0x0A] = (builtin_func_t)get_elapsed_time;
    builtin_table[0x0B] = (builtin_func_t)free_all_timers;
    builtin_table[0x0C] = (builtin_func_t)vm_thread_spawn;
    builtin_table[0x0D] = (builtin_func_t)vm_thread_pause;
    builtin_table[0x0E] = (builtin_func_t)vm_thread_resume;
    builtin_table[0x0F] = (builtin_func_t)vm_thread_kill;
    builtin_table[0x10] = (builtin_func_t)vm_thread_yield;

    // 预留扩展位，假设还有 0x11 ~ 0x22
    builtin_table[0x11] = (builtin_func_t)empty;
    builtin_table[0x12] = (builtin_func_t)empty;
    builtin_table[0x13] = (builtin_func_t)empty;
    builtin_table[0x14] = (builtin_func_t)empty;
    builtin_table[0x15] = (builtin_func_t)empty;
    builtin_table[0x16] = (builtin_func_t)empty;
    builtin_table[0x17] = (builtin_func_t)empty;
    builtin_table[0x18] = (builtin_func_t)empty;
    builtin_table[0x19] = (builtin_func_t)empty;
    builtin_table[0x1A] = (builtin_func_t)empty;
    builtin_table[0x1B] = (builtin_func_t)empty;
    builtin_table[0x1C] = (builtin_func_t)empty;
    builtin_table[0x1D] = (builtin_func_t)empty;
    builtin_table[0x1E] = (builtin_func_t)empty;
    builtin_table[0x1F] = (builtin_func_t)empty;
    builtin_table[0x20] = (builtin_func_t)empty;
    builtin_table[0x21] = (builtin_func_t)empty;
    builtin_table[0x22] = (builtin_func_t)empty;
}
