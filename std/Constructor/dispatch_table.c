#include <stdio.h>

void func0() { printf(">> Function 0 executed\n"); }
void func1() { printf(">> Function 1 executed\n"); }
void func2() { printf(">> Function 2 executed\n"); }

void (*dispatch_table[])() = {
    func0,
    func1,
    func2
};