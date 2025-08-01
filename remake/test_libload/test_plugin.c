#include <stdio.h>
#include <string.h>
#include "test_plugin.h"

// 基础类型：无返回
void test1_int(int x) {
    printf("[PLUGIN] test1_int called, arg=%d\n", x);
}

// 基础类型：有返回
int test2_add(int a, int b) {
    printf("[PLUGIN] test2_add called, %d + %d\n", a, b);
    return a + b;
}

// 字符串输入
void test3_str(const char* msg) {
    printf("[PLUGIN] test3_str called, msg=%s\n", msg);
}

// 返回 struct
MyStruct test4_make_struct(int id, double val, const char* name) {
    MyStruct s;
    s.id = id;
    s.value = val;
    strncpy(s.name, name, sizeof(s.name)-1);
    printf("[PLUGIN] test4_make_struct called\n");
    return s;
}

// 返回 struct 指针（动态分配）
MyStruct* test5_make_struct_ptr(int id, double val, const char* name) {
    MyStruct* s = (MyStruct*)malloc(sizeof(MyStruct));
    s->id = id;
    s->value = val;
    strncpy(s->name, name, sizeof(s->name)-1);
    printf("[PLUGIN] test5_make_struct_ptr called\n");
    return s;
}

