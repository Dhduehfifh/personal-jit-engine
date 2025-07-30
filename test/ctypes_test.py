# ctypes_test.py
import ctypes

lib = ctypes.CDLL("./libcore.so")

# ===== Alloc / Free
lib.alloc_mem.restype = ctypes.c_void_p
ptr = lib.alloc_mem(64)

# 写入数据
data = b"hello world"
lib.write_data(ptr, 0, data, len(data))

# 遍历打印
lib.dump_mem(ptr, 64)

# ===== 普通栈使用
lib.stack_create.restype = ctypes.c_void_p
stack = lib.stack_create(32)

lib.stack_push(stack, 0x10)
lib.stack_push(stack, 0x20)
print("Pop:", lib.stack_pop(stack))
lib.stack_free(stack)

# ===== 输入输出
buffer = ctypes.create_string_buffer(128)
lib.write_output(b"Hello From Python")
# lib.read_input(buffer, 128)
# print("You Typed:", buffer.value)

# ===== 汇编导出函数调用
align_ptr = lib.align_ptr
align_ptr.restype = ctypes.c_void_p
align_ptr.argtypes = [ctypes.c_void_p]
print("Aligned:", hex(align_ptr(123)))

add_offset = lib.add_offset
add_offset.restype = ctypes.c_void_p
add_offset.argtypes = [ctypes.c_void_p, ctypes.c_size_t]
print("Offset:", hex(add_offset(1000, 24)))

lib.call_libc_puts.argtypes = [ctypes.c_char_p]
lib.call_libc_puts(b"Called puts from asm!")
