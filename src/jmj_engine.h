#ifndef JMJ_ENGINE_H
#define JMJ_ENGINE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 初始化引擎及 dispatch 表
void jmj_engine_init(void);

// 清理资源
void jmj_engine_shutdown(void);

// dispatch 调用入口
void jmj_dispatch(uint8_t opcode, void* ctx);

// 获取错误日志路径
const char* jmj_get_error_log_path(void);

// 获取当前线程 ID（用于调试或线程级权限控制）
uint64_t jmj_get_current_thread_id(void);

// 检查 opcode 是否是合法的函数
int jmj_is_valid_opcode(uint8_t opcode);

#ifdef __cplusplus
}
#endif

#endif // JMJ_ENGINE_H
