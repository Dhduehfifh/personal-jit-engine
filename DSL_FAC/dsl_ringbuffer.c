// dsl_ringbuffer.c
#include "dsl_ringbuffer.h"
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <string.h>

typedef void* void_ptr;

struct RingBuffer {
    _Atomic(void_ptr)* buffer;
    _Atomic(size_t) head;
    _Atomic(size_t) tail;
    size_t capacity;
    _Atomic(size_t) count;
} __attribute__((aligned(CACHE_LINE)));

RingBuffer* rb_create(size_t capacity) {
    RingBuffer* rb;
    if (posix_memalign((void**)&rb, CACHE_LINE, sizeof(RingBuffer)) != 0) return NULL;
    memset(rb, 0, sizeof(RingBuffer));
    rb->buffer = (_Atomic(void_ptr)*)calloc(capacity, sizeof(_Atomic(void_ptr)));
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }
    atomic_store(&rb->head, 0);
    atomic_store(&rb->tail, 0);
    rb->capacity = capacity;
    atomic_store(&rb->count, 0);
    return rb;
}

void rb_destroy(RingBuffer* rb) {
    if (rb) {
        free(rb->buffer);
        free(rb);
    }
}

int rb_push(RingBuffer* rb, void_ptr item) {
    size_t count = atomic_load(&rb->count);
    if (count >= rb->capacity) return -1;

    size_t tail = atomic_fetch_add(&rb->tail, 1) % rb->capacity;
    atomic_store(&rb->buffer[tail], item);
    atomic_fetch_add(&rb->count, 1);
    return 0;
}

int rb_pop(RingBuffer* rb, void_ptr* out) {
    size_t count = atomic_load(&rb->count);
    if (count == 0) return -1;

    size_t head = atomic_fetch_add(&rb->head, 1) % rb->capacity;
    void_ptr item = atomic_load(&rb->buffer[head]);
    *out = item;
    atomic_fetch_sub(&rb->count, 1);
    return 0;
}

// -----------------------------------------
// DSL 指令流水线结构与实现

typedef struct {
    dsl_func_t func;
    void_ptr arg;
} Task;

struct DslPipeline {
    RingBuffer* queue;
    pthread_t thread;
    _Atomic int running;
};

void* dsl_pipeline_loop(void* arg) {
    DslPipeline* pipe = (DslPipeline*)arg;
    Task* task;

    while (atomic_load(&pipe->running)) {
        void_ptr raw;
        if (rb_pop(pipe->queue, &raw) == 0) {
            task = (Task*)raw;
            if (task->func) task->func(task->arg);
            free(task); // 清理任务结构
        } else {
            // 空闲时可挂起线程（简单 sleep 模式）
            struct timespec ts = {0, 1000000}; // 1ms
            nanosleep(&ts, NULL);
        }
    }
    return NULL;
}

DslPipeline* dsl_pipeline_create(size_t capacity) {
    DslPipeline* pipe = calloc(1, sizeof(DslPipeline));
    if (!pipe) return NULL;
    pipe->queue = rb_create(capacity);
    atomic_store(&pipe->running, 0);
    return pipe;
}

void dsl_pipeline_destroy(DslPipeline* pipe) {
    if (!pipe) return;
    dsl_pipeline_stop(pipe);
    rb_destroy(pipe->queue);
    free(pipe);
}

int dsl_pipeline_submit(DslPipeline* pipe, dsl_func_t func, void_ptr arg) {
    Task* task = malloc(sizeof(Task));
    if (!task) return -1;
    task->func = func;
    task->arg = arg;
    return rb_push(pipe->queue, task);
}

void dsl_pipeline_start(DslPipeline* pipe) {
    if (atomic_load(&pipe->running)) return;
    atomic_store(&pipe->running, 1);
    pthread_create(&pipe->thread, NULL, dsl_pipeline_loop, pipe);
}

void dsl_pipeline_stop(DslPipeline* pipe) {
    if (!atomic_load(&pipe->running)) return;
    atomic_store(&pipe->running, 0);
    pthread_join(pipe->thread, NULL);
}
