#include <stdio.h>

// 供外部调用的示例函数1
int example_func1(void* arg) {
    (void)arg;
    printf("[libexample2] example_func1 called!\n");
    return 0;
}

// 供外部调用的示例函数2
int example_func2(void* arg) {
    (void)arg;
    printf("[libexample] example_func2 called!\n");
    return 0;
}
