import ctypes
import sys

# 增强版：使用虚拟内存技巧
shellcode = b"\x48\xC7\xC0\x2A\x00\x00\x00\xC3"

# 在堆上分配内存
buf = (ctypes.c_char * len(shellcode)).from_buffer_copy(shellcode)

# 关键技巧：强制类型转换
func_type = ctypes.CFUNCTYPE(ctypes.c_long)
ctypes.memmove(ctypes.addressof(buf), shellcode, len(shellcode))
func = func_type(ctypes.addressof(buf))

print("Result:", func())  # 应该输出42