#include <stdio.h>
#include <stdint.h>
#include <time.h>

extern int asm_ifelse(int x);
extern int asm_math(int x);

#define REPEAT 10000000

static inline double now_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000.0 + ts.tv_nsec / 1000000.0;
}

int main() {
    int sum = 0;

    double start = now_ms();
    for (int i = 0; i < REPEAT; i++) {
        sum += asm_ifelse(42);
    }
    double end = now_ms();
    printf("[ifelse] time: %.3f ms, result = %d\n", end - start, sum);

    sum = 0;
    start = now_ms();
    for (int i = 0; i < REPEAT; i++) {
        sum += asm_math(42);
    }
    end = now_ms();
    printf("[math   ] time: %.3f ms, result = %d\n", end - start, sum);

    return 0;
}