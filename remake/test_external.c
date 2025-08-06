#include <stdint.h>
#include <stdio.h>

__attribute__((visibility("default")))
void external_add_one(uint64_t *args, uint64_t argc) {
    if (argc < 2) return;

    uint64_t *arr = (uint64_t*)args[0]; // 第一个参数是数组指针
    uint64_t size = args[1];           // 第二个参数是数组大小

    printf("[external_add_one] Called with arr=%p, size=%llu\n",
           (void*)arr, (unsigned long long)size);

    for (uint64_t i = 0; i < size; i++) {
        arr[i] += 1;
    }
}
