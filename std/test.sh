#!/bin/bash
# 测试内存分配/释放
for i in {1..1000}; do
    ./build/uranium_jit --test alloc_free || exit 1
done

# 检查内存泄漏
valgrind --leak-check=full --error-exitcode=1 \
    ./build/uranium_jit --test memory_stress