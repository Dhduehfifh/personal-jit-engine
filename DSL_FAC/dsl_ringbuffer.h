// dsl_ringbuffer.h
#ifndef DSL_RINGBUFFER_H
#define DSL_RINGBUFFER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CACHE_LINE 64

typedef void* void_ptr;

// 工厂函数定义（DSL 指令流水线接口）
typedef void (*dsl_func_t)(void_ptr arg);

typedef struct RingBuffer RingBuffer;
typedef struct DslPipeline DslPipeline;

// RingBuffer 接口
RingBuffer* rb_create(size_t capacity);
void rb_destroy(RingBuffer* rb);
int rb_push(RingBuffer* rb, void_ptr item);
int rb_pop(RingBuffer* rb, void_ptr* out);

// DSL Pipeline 接口
DslPipeline* dsl_pipeline_create(size_t capacity);
void dsl_pipeline_destroy(DslPipeline* pipe);
int dsl_pipeline_submit(DslPipeline* pipe, dsl_func_t func, void_ptr arg);
void dsl_pipeline_start(DslPipeline* pipe);
void dsl_pipeline_stop(DslPipeline* pipe);

#ifdef __cplusplus
}
#endif
#endif // DSL_RINGBUFFER_H
