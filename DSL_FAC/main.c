// main.c
#include "dsl_ringbuffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void my_task(void_ptr arg) {
    char* msg = (char*)arg;
    printf("Executing task: %s\n", msg);
    free(msg);
}

int main() {
    DslPipeline* pipeline = dsl_pipeline_create(64);
    dsl_pipeline_start(pipeline);

    for (int i = 0; i < 10; ++i) {
        char* msg = malloc(64);
        snprintf(msg, 64, "Task #%d", i);
        dsl_pipeline_submit(pipeline, my_task, msg);
    }

    // 等待任务执行完成
    sleep(1);

    dsl_pipeline_stop(pipeline);
    dsl_pipeline_destroy(pipeline);
    return 0;
}
