#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   // 用于计时
#include "dispatch.h"

#define BUFFER_LEN 5

static uint64_t get_external_opcode(int i) {
    uint64_t sample_opcodes[] = {0x00, 0x01, 0x05, 0x0C, 0x10};
    return sample_opcodes[i % (sizeof(sample_opcodes) / sizeof(sample_opcodes[0]))];
}

static double timespec_diff_sec(struct timespec* start, struct timespec* end) {
    return (double)(end->tv_sec - start->tv_sec) + 
           (double)(end->tv_nsec - start->tv_nsec) / 1e9;
}

int main() {
    printf("=== Main 调试版开始 ===\n");

    printf("[1] 分配缓冲区（长度=%d）...\n", BUFFER_LEN);
    uint64_t buffer[BUFFER_LEN];
    memset(buffer, 0, sizeof(buffer));
    printf("[1] 缓冲区地址: %p, 大小: %zu 字节\n", (void*)buffer, sizeof(buffer));

    printf("\n[2] 从外部接收数据...\n");
    for (int i = 0; i < BUFFER_LEN; i++) {
        buffer[i] = get_external_opcode(i);
        printf("    接收到 opcode[%d] = 0x%02llX\n", i, (unsigned long long)buffer[i]);
    }

    printf("\n[3] 压入栈（直接写入缓冲区）...\n");
    for (int i = 0; i < BUFFER_LEN; i++) {
        printf("    栈位置[%d] <- 0x%02llX\n", i, (unsigned long long)buffer[i]);
    }

    printf("\n[4] 遍历执行 opcode...\n");

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    for (int i = 0; i < BUFFER_LEN; i++) {
        printf("    [执行前] i=%d, opcode=0x%02llX\n", i, (unsigned long long)buffer[i]);
        dispatch_exec(buffer[i]);
        printf("    [执行后] i=%d 完成\n", i);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);

    double elapsed = timespec_diff_sec(&start_time, &end_time);
    printf("\n[5] opcode 遍历执行总耗时: %.9f 秒\n", elapsed);

    printf("[6] 所有 opcode 执行完毕，退出程序。\n");
    printf("=== Main 调试版结束 ===\n");
    return 0;
}
