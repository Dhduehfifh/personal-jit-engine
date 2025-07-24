#include "jmj_engine.h"
#include "dispatch_table.h"   // 你的虚表和 dispatch_entry 实现

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// 错误日志文件信息
static const char* log_path = "/tmp/jmj_error.log";
static FILE* log_file = NULL;
static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

// 错误日志线程
static void* error_logger_thread(void* arg) {
    pthread_mutex_lock(&log_mutex);
    log_file = fopen(log_path, "a");
    if (!log_file) {
        pthread_mutex_unlock(&log_mutex);
        return NULL;
    }
    time_t now = time(NULL);
    fprintf(log_file, "\n=== JMJ 引擎启动: %s", ctime(&now));
    fflush(log_file);
    pthread_mutex_unlock(&log_mutex);
    return NULL;
}

const char* jmj_get_error_log_path(void) {
    return log_path;
}

uint64_t jmj_get_current_thread_id(void) {
    return (uint64_t)pthread_self();
}

// 初始化引擎
void jmj_engine_init(void) {
    pthread_t tid;
    pthread_create(&tid, NULL, error_logger_thread, NULL);
    pthread_detach(tid);

    init_builtin_dispatch();  // 初始化虚表调度器
    init_stack_factory();     // 初始化参数栈系统（如果需要）
    fprintf(stderr, "[JMJ] 引擎初始化完成。\n");
}

// 清理资源
void jmj_engine_shutdown(void) {
    pthread_mutex_lock(&log_mutex);
    if (log_file) {
        time_t now = time(NULL);
        fprintf(log_file, "=== JMJ 引擎关闭: %s", ctime(&now));
        fclose(log_file);
        log_file = NULL;
    }
    pthread_mutex_unlock(&log_mutex);
    fprintf(stderr, "[JMJ] 引擎已关闭。\n");
}

// 检查是否合法 opcode（0 表示非法）
int jmj_is_valid_opcode(uint8_t opcode) {
    return dispatch_has_entry(opcode);  // 你需要在 dispatch_table.c 中实现这个函数
}

// dispatch 接口
void jmj_dispatch(uint8_t opcode, void* ctx) {
    if (jmj_is_valid_opcode(opcode)) {
        dispatch_entry(opcode, ctx);
    } else {
        stack_factory_push(ctx);  // 不合法就当作参数丢入栈工厂
        pthread_mutex_lock(&log_mutex);
        if (log_file) {
            fprintf(log_file, "[WARN] 非法 opcode: %d，已作为参数处理。\n", opcode);
            fflush(log_file);
        }
        pthread_mutex_unlock(&log_mutex);
    }
}
