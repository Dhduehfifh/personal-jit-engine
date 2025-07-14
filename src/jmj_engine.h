#ifndef JMJ_ENGINE_H
#define JMJ_ENGINE_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ===== VM 状态初始化和关闭 =====
int jmj_init(size_t page_size);           // 初始化虚拟机、分配页面
void jmj_shutdown(void);                  // 关闭 VM 清理资源

// ===== Dispatch 初始化入口（必须调用）=====
void init_builtin_dispatch(void);         // 注册 dispatch opcode 函数表

// ===== 单条指令推送 =====
int jmj_push_opcode(uint8_t opcode);      // 推送单条 opcode，自动判断是否函数分发

// ===== 字节流批量推送 =====
int jmj_push_bytes(const uint8_t* buf, size_t len); // 批量推送 opcode 字节码

// ===== 运行入口（可选）=====
int jmj_execute(void);                    // 手动触发调度循环（若需）

// ===== 获取结果 =====
typedef struct {
    void* data;
    size_t size;
} jmj_result_t;

jmj_result_t jmj_get_result(void);        // 获取运算/结构处理后的数据

// ===== 注册日志与 panic 回调 =====
typedef void (*jmj_log_cb)(const char*);
typedef void (*jmj_panic_cb)(uint32_t code);

void jmj_set_log_callback(jmj_log_cb fn);
void jmj_set_panic_callback(jmj_panic_cb fn);

#ifdef __cplusplus
}
#endif

#endif // JMJ_ENGINE_H
