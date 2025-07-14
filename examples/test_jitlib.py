from ctypes import CDLL, c_uint8, c_size_t, POINTER, Structure

lib = CDLL("./libjmj.so")

# 初始化
lib.jmj_init(c_size_t(4096))
lib.init_builtin_dispatch()

# 发送指令
lib.jmj_push_opcode(0x01)
lib.jmj_push_opcode(0x02)
lib.jmj_push_opcode(0x03)

# 获取结果结构体
class JMJResult(Structure):
    _fields_ = [("data", POINTER(c_uint8)), ("size", c_size_t)]

lib.jmj_get_result.restype = JMJResult
res = lib.jmj_get_result()
print("返回大小：", res.size)
