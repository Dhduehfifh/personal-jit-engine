.section __TEXT,__text
.global _write_some_data

// void write_some_data();
_write_some_data:
    // 获取写入指针
    bl _get_write_ptr
    mov x1, x0        // x1 = buffer ptr

    // 写入固定数据
    mov w2, #'{'      // ASCII {
    strb w2, [x1]
    mov w2, #'M'
    strb w2, [x1, #1]
    mov w2, #'V'
    strb w2, [x1, #2]
    mov w2, #'}'
    strb w2, [x1, #3]

    // 通知提交
    mov x0, x1        // ptr
    mov x1, #4        // len
    bl _commit_write
    ret