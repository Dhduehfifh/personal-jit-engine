#include "dispatch.h"
#include "bulitin_func.h"
#include <string.h>
#include <stdio.h>

static dispatch_func_t dispatch_table[BUFFER_MAX];

/**
 * 初始化 dispatch 表
 */
void dispatch_init(void) {
    memset(dispatch_table, 0, sizeof(dispatch_table));
}

/**
 * 注册单个函数
 */
void dispatch_register(uint64_t opcode, dispatch_func_t func) {
    if (opcode >= BUFFER_MAX) {
        printf("[dispatch] opcode %llu 超过 BUFFER_MAX=%d\n",
               (unsigned long long)opcode, BUFFER_MAX);
        return;
    }
    dispatch_table[opcode] = func;
}

/**
 * 自动注册 bulitin_func.h 中声明的所有内建函数
 * 注意：这里只是硬编码顺序映射，保持和 VM 内的编号一致
 */
void dispatch_register_all_builtin(void) {
    uint64_t id = 0;

    // 内存管理
    dispatch_register(id++, (dispatch_func_t)alloc);
    dispatch_register(id++, (dispatch_func_t)release);
    dispatch_register(id++, (dispatch_func_t)terminate);

    // 比较与跳转
    dispatch_register(id++, (dispatch_func_t)cmp_only);
    dispatch_register(id++, (dispatch_func_t)cmp_eq);
    dispatch_register(id++, (dispatch_func_t)cmp_not_eq);
    dispatch_register(id++, (dispatch_func_t)jump_to);

    // 算术
    dispatch_register(id++, (dispatch_func_t)add_u64);
    dispatch_register(id++, (dispatch_func_t)sub_u64);
    dispatch_register(id++, (dispatch_func_t)mul_u64);
    dispatch_register(id++, (dispatch_func_t)div_u64);

    // 数据移动
    dispatch_register(id++, (dispatch_func_t)move);
    dispatch_register(id++, (dispatch_func_t)copy);

    // 获取时间戳
    //dispatch_register(id++, (dispatch_func_t)get_tsc);

    // 计时器
    dispatch_register(id++, (dispatch_func_t)timer_start_now);
    dispatch_register(id++, (dispatch_func_t)timer_end_now);
    dispatch_register(id++, (dispatch_func_t)timer_start_segment);
    dispatch_register(id++, (dispatch_func_t)timer_end_segment);

    // 线程
    dispatch_register(id++, (dispatch_func_t)vm_thread_spawn);
    dispatch_register(id++, (dispatch_func_t)vm_thread_join);
    dispatch_register(id++, (dispatch_func_t)vm_thread_kill);
    dispatch_register(id++, (dispatch_func_t)vm_thread_yield);

    // 动态库调用
    dispatch_register(id++, (dispatch_func_t)builtin_mount_dynamic_lib);
    dispatch_register(id++, (dispatch_func_t)builtin_call_dynamic_func);

    printf("[dispatch] 已注册 %llu 个内建函数\n", (unsigned long long)id);
}

/**
 * 执行程序
 * program: uint64_t 数组
 * start_ptr: 起始位置
 * end_ptr: 结束位置
 */
void dispatch_execute(uint64_t *program, uint64_t start_ptr, uint64_t end_ptr) {
    uint64_t pc = start_ptr;

    while (pc < end_ptr) {
        uint64_t opcode_raw = program[pc++];
        int64_t opcode = (int64_t)opcode_raw - (int64_t)CONST_NUMBER;

        if (opcode >= 0 && opcode < BUFFER_MAX) {
            dispatch_func_t func = dispatch_table[opcode];
            if (!func) {
                printf("[dispatch] opcode=%lld 未注册函数\n", (long long)opcode);
                continue;
            }

            uint64_t args[MAX_ARGS] = {0};
            size_t arg_count = 0;

            // 取到下一个 opcode 之前的值作为参数
            while (pc < end_ptr && arg_count < MAX_ARGS) {
                int64_t maybe_opcode = (int64_t)program[pc] - (int64_t)CONST_NUMBER;
                if (maybe_opcode >= 0 && maybe_opcode < BUFFER_MAX) {
                    break; // 碰到下一个 opcode
                }
                args[arg_count++] = program[pc++];
            }

            func(args[0], args[1], args[2], args[3],
                 args[4], args[5], args[6], args[7]);
        } else {
            printf("[dispatch] 无效 opcode_raw=%llu\n",
                   (unsigned long long)opcode_raw);
        }
    }
}
