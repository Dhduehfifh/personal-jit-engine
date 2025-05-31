#include <stdio.h>
#include <stdlib.h>

// 正确用法：声明的是 C 层逻辑名，不加下划线！
extern void* create_object(int a, int b);
extern int dispatch_call(int idx);

int main() {
    void* obj = create_object(42, 99);
    int* data = (int*)obj;
    printf("Object created: a=%d, b=%d\n", data[0], data[1]);

    printf("Calling dispatch function 0:\n");
    dispatch_call(0);

    printf("Calling dispatch function 2:\n");
    dispatch_call(2);

    free(obj);
    return 0;
}