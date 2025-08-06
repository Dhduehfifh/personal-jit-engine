from ctypes import cdll, c_uint64, c_int, POINTER

# ===== 加载动态库 =====
lib = cdll.LoadLibrary("./build/libjmj.so")

# ===== 设置接口返回类型 / 参数类型 =====
lib.jitvm_bios_boot.restype = c_int
lib.jit_capi_boot.restype = c_int
lib.jit_capi_exec_array.restype = None
lib.jit_capi_shutdown.restype = None
lib.jitvm_bios_shutdown.restype = None

lib.jit_capi_get_retval.restype = c_uint64
lib.jit_capi_get_retval.argtypes = [c_int]

lib.jit_capi_ret_get_data.restype = c_uint64
lib.jit_capi_ret_get_data.argtypes = [c_uint64]

# ===== 启动 BIOS / CAPI =====
print("[Python] 启动 BIOS/CAPI")
lib.jitvm_bios_boot()
lib.jit_capi_boot()

# ===== 内建函数参数数量表（不包括动态库） =====
builtin_argc = {
    0x00: 0,  # empty
    0x01: 2,  # alloc(size, ctx_ptr)
    0x02: 3,  # release(size, ctx_ptr, release_stack_head_ptr)
    0x03: 1,  # terminate(ctx_ptr)
    0x04: 2,  # compare(a, b)
    0x05: 1,  # jump_if_eq(target)
    0x06: 1,  # jump_if_neq(target)
    0x07: 1,  # jump(target)
    0x08: 1,  # start_timer(id)
    0x09: 1,  # end_timer(id)
    0x0A: 1,  # get_elapsed_time(id)
    0x0B: 0,  # free_all_timers()
    0x0C: 0,  # vm_thread_spawn()
    0x0D: 0,  # vm_thread_pause()
    0x0E: 0,  # vm_thread_resume()
    0x0F: 0,  # vm_thread_kill()
    0x10: 0,  # vm_thread_yield()
}

# ===== 指令流缓冲区 =====
stream = []

def append_op(op, *args):
    """带参数数量检查"""
    expected = builtin_argc.get(op, None)
    if expected is None:
        raise ValueError(f"未知 opcode: {op:#x}")
    if len(args) != expected:
        raise ValueError(f"Opcode {op:#x} 参数错误: 需要 {expected} 个, 实际 {len(args)} 个")
    stream.extend([int(op)] + [int(a) for a in args])

def run_and_get_ret(slot=0):
    """执行并从返回值池中读取"""
    data_stream = (c_uint64 * len(stream))(*stream)
    lib.jit_capi_exec_array(data_stream, len(stream))
    ret_id = lib.jit_capi_get_retval(slot)
    value = lib.jit_capi_ret_get_data(ret_id)  # 统一从池读取
    stream.clear()
    return value

# ===== 测试所有内建函数 =====
print("\n[Python] 开始批量测试所有内建函数（非动态库）")

'''append_op(0x00)
print(f"empty() -> {run_and_get_ret()}")

append_op(0x01, 64, 0x1234)
print(f"alloc(64, 0x1234) -> {run_and_get_ret()}")

append_op(0x02, 1, 0x0, 0x1234)
print(f"release(...) -> {run_and_get_ret()}")

append_op(0x03, 0x1234)
print(f"terminate(...) -> {run_and_get_ret()}")'''

append_op(0x04, 10, 10)
print(f"compare(10, 10) -> {run_and_get_ret()}")

append_op(0x05, 0x1234)
print(f"jump_if_eq(...) -> {run_and_get_ret()}")
'''
append_op(0x06, 0x1234)
print(f"jump_if_neq(...) -> {run_and_get_ret()}")'''
'''
append_op(0x07, 0x1234)
print(f"jump(...) -> {run_and_get_ret()}")'''
'''
append_op(0x08, 1)
print(f"start_timer(1) -> {run_and_get_ret()}")

append_op(0x09, 1)
print(f"end_timer(1) -> {run_and_get_ret()}")

append_op(0x0A, 1)
print(f"get_elapsed_time(1) -> {run_and_get_ret()}")

append_op(0x0B)
print(f"free_all_timers() -> {run_and_get_ret()}")'''

append_op(0x0C)
print(f"vm_thread_spawn() -> {run_and_get_ret()}")

append_op(0x0D)
print(f"vm_thread_pause() -> {run_and_get_ret()}")

append_op(0x0E)
print(f"vm_thread_resume() -> {run_and_get_ret()}")

append_op(0x0F)
print(f"vm_thread_kill() -> {run_and_get_ret()}")

append_op(0x10)
print(f"vm_thread_yield() -> {run_and_get_ret()}")

print("\n[Python] 测试完成")
