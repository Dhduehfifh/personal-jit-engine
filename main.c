#include <stdio.h>

// 汇编中导出的函数符号要加下划线
extern int _my_add(int a, int b);

int main() {
    int result = _my_add(21, 21);
    printf("Result = %d\n", result);
    return 0;
}