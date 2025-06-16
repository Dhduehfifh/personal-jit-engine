#include <stdio.h>
#include <stdint.h>

extern int asm_ifelse(int x);
extern int asm_math(int x);

int main() {
    int test_val = 42;

    int res_if = asm_ifelse(test_val);
    printf("[ifelse] result = %d\n", res_if);

    int res_math = asm_math(test_val);
    printf("[math  ] result = %d\n", res_math);

    return 0;
}