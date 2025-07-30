import ctypes
import mmap
import os
import sys

# 加载 libc
if sys.platform == "linux":
    libc = ctypes.CDLL("libc.so.6")
elif sys.platform == "darwin":
    libc = ctypes.CDLL("libc.dylib")
else:
    raise RuntimeError("Unsupported OS")

# ---------------------
# 1. malloc / free
# ---------------------
malloc = libc.malloc
malloc.restype = ctypes.c_void_p
malloc.argtypes = [ctypes.c_size_t]

free = libc.free
free.restype = None
free.argtypes = [ctypes.c_void_p]

# ---------------------
# 2. 写入任意数据
# ---------------------
def write_data(ptr, offset, data: bytes):
    """
    将任意 bytes 数据写入指定内存地址。
    """
    target = ctypes.cast(ptr + offset, ctypes.POINTER(ctypes.c_char * len(data)))
    ctypes.memmove(target, data, len(data))

# ---------------------
# 3. 遍历内存块
# ---------------------
def traverse_memory(ptr, size):
    """
    将内存块读取为 bytes。
    """
    raw = ctypes.cast(ptr, ctypes.POINTER(ctypes.c_char * size))
    return bytes(raw.contents)

# ---------------------
# 4. 普通栈 & 环形栈
# ---------------------
class Stack:
    def __init__(self, capacity):
        self.ptr = malloc(capacity)
        self.cap = capacity
        self.top = 0

    def push(self, data: bytes):
        if self.top + len(data) > self.cap:
            raise MemoryError("Stack overflow")
        write_data(self.ptr, self.top, data)
        self.top += len(data)

    def pop(self, size) -> bytes:
        if self.top < size:
            raise MemoryError("Stack underflow")
        self.top -= size
        return traverse_memory(self.ptr + self.top, size)

    def destroy(self):
        free(self.ptr)

class CircularStack:
    def __init__(self, capacity):
        self.ptr = malloc(capacity)
        self.cap = capacity
        self.head = 0
        self.tail = 0

    def push(self, data: bytes):
        for b in data:
            self.ptr[self.tail % self.cap] = b
            self.tail += 1

    def pop(self, size) -> bytes:
        result = bytearray()
        for _ in range(size):
            result.append(self.ptr[self.head % self.cap])
            self.head += 1
        return bytes(result)

    def destroy(self):
        free(self.ptr)

# ---------------------
# 5. 动态加载库 + 函数列表
# ---------------------
import glob
import subprocess

def list_libc_functions():
    """
    尝试 nm 或 objdump 获取 libc 可导出函数（需安装 binutils）。
    """
    libc_path = libc._name
    try:
        output = subprocess.check_output(['nm', '-D', libc_path], stderr=subprocess.DEVNULL).decode()
    except FileNotFoundError:
        output = subprocess.check_output(['objdump', '-T', libc_path], stderr=subprocess.DEVNULL).decode()

    func_list = []
    for line in output.splitlines():
        if ' T ' in line or ' t ' in line:
            parts = line.strip().split()
            func = parts[-1]
            func_list.append(func)
    return func_list

# ---------------------
# 6. 指针偏移 & 函数指针
# ---------------------
def pointer_offset(ptr, offset, type_=ctypes.c_void_p):
    return ctypes.cast(ptr + offset, ctypes.POINTER(type_))

def call_func_ptr(ptr, restype, argtypes, *args):
    func_type = ctypes.CFUNCTYPE(restype, *argtypes)
    func = func_type(ptr)
    return func(*args)

# 示例：通过 ctypes 函数指针调用 puts
def call_puts_from_ptr():
    puts_ptr = ctypes.cast(libc.puts, ctypes.c_void_p).value
    msg = ctypes.create_string_buffer(b"Hello from puts via pointer!")
    return call_func_ptr(puts_ptr, ctypes.c_int, [ctypes.c_char_p], ctypes.byref(msg))

# ---------------------
# 7. input/output 辅助
# ---------------------
def c_input(prompt: str) -> str:
    print(prompt, end='', flush=True)
    buf = ctypes.create_string_buffer(256)
    libc.fgets(buf, 256, libc.stdin)
    return buf.value.decode()

def c_output(msg: str):
    libc.printf(b"%s\n", msg.encode())

# ---------------------
# 8. 示例测试用 main()
# ---------------------
if __name__ == "__main__":
    print("Allocating 32 bytes...")
    p = malloc(32)
    write_data(p, 0, b"ABCDEF123456")
    print("Memory:", traverse_memory(p, 12))
    free(p)

    print("Calling puts via pointer:")
    call_puts_from_ptr()

    print("Listing libc functions (partial):")
    print(list_libc_functions()[:10])
