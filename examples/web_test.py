import socket
import time

bytecode = bytes([0x01, 0x02, 0x03])

time.sleep(1)  # 等待服务端完成 listen

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print("======== started ========")
    s.connect(("127.0.0.1", 8080))  # 本地连接
    print("======== connected ========")
    s.sendall(bytecode)
    print("======== sent ========")
    data = s.recv(4096)
    print("======== received ========")
    print("data:", data)
