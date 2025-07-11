#ifndef JMJ_ENGINE_H
#define JMJ_ENGINE_H

#include <stdint.h>
#include <stdlib.h>

// 初始化和销毁
int jmj_init(size_t code_page_size);
void jmj_reset(void);
void jmj_shutdown(void);

// 字节码载入与编译
int jmj_compile(const uint8_t *bytecode, size_t len);

// 执行 (run 或 step) 
int jmj_execute(void);

// 设置 panic/log 回调
typedef void (*jmj_log_cb)(const char*);
typedef void (*jmj_panic_cb)(uint32_t code);
void jmj_set_log_callback(jmj_log_cb fn);
void jmj_set_panic_callback(jmj_panic_cb fn);

// 获取执行返回值 (示例结构)
typedef struct {
  void *data;
  size_t size;
} jmj_result_t;
jmj_result_t jmj_get_result(void);

#endif // JMJ_ENGINE_H
