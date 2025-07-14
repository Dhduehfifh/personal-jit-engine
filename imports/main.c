#include "studio.h"
#include <stdio.h>

int main() {
    studio_init();

    // === 加载第一个动态库 ===
    if (studio_switch_library("./libexample.so") != 0) {
        printf("加载 libexample.so 失败\n");
        return 1;
    }

    const char* func1 = "example_func1";
    printf("调用 libexample.so 中的函数: %s\n", func1);
    if (studio_call_function(func1, NULL) != 0) {
        printf("调用 %s 失败\n", func1);
    }

    // === 切换加载第二个动态库 ===
    if (studio_switch_library("./libexample2.so") != 0) {
        printf("加载 libexample2.so 失败\n");
        return 1;
    }

    const char* func2 = "example_func2";
    printf("调用 libexample2.so 中的函数: %s\n", func2);
    if (studio_call_function(func2, NULL) != 0) {
        printf("调用 %s 失败\n", func2);
    }

    studio_cleanup();
    return 0;
}
