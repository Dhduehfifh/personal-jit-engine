import ctypes
import os

# 加载动态库
lib = ctypes.CDLL('./libjit.dylib')

# === 声明导出的函数 ===

# void example_event_export()
lib.example_event_export.restype = None

# void register_handler_export(uint8_t, void*)
lib.register_handler_export.argtypes = [ctypes.c_uint8, ctypes.c_void_p]
lib.register_handler_export.restype = None

# void* asm_dispatch_export(uint8_t)
lib.asm_dispatch_export.argtypes = [ctypes.c_uint8]
lib.asm_dispatch_export.restype = ctypes.c_void_p

# void* resolve_handler_export(uint8_t)
lib.resolve_handler_export.argtypes = [ctypes.c_uint8]
lib.resolve_handler_export.restype = ctypes.c_void_p

# === 注册和调用过程 ===

# 步骤 1：获取 example_event_export 的函数指针
example_event_ptr = ctypes.cast(lib.example_event_export, ctypes.c_void_p)

# 步骤 2：注册 handler 到 opcode=0x01
lib.register_handler_export(0x01, example_event_ptr)

# 步骤 3：通过汇编桥接函数获取 handler 指针
handler_ptr = lib.asm_dispatch_export(0x01)

# 步骤 4：构造 Python 可调用函数对象（void () 类型）
handler_fn = ctypes.CFUNCTYPE(None)(handler_ptr)

# 步骤 5：调用该函数
print("Calling opcode 0x01 handler...")
handler_fn()