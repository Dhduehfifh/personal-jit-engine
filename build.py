import os
import platform
import subprocess
import sys
from pathlib import Path

SRC_DIR = "assembly"
BUILD_DIR = "build"
EXECUTABLE = f"{BUILD_DIR}/demo"

def detect_platform():
    system = platform.system()
    if system == "Darwin":
        return "mac"
    elif system == "Linux":
        return "linux"
    elif system == "Windows":
        return "win"
    else:
        raise RuntimeError(f"Unsupported OS: {system}")

def compile_asm_mac(asm_path, obj_path):
    print(f"[🧩] Compiling {asm_path} (macOS clang AT&T)")
    subprocess.run(["as", "-o", obj_path, asm_path], check=True)

def build():
    os.makedirs(BUILD_DIR, exist_ok=True)
    system = detect_platform()

    obj_files = []

    # 遍历所有汇编文件
    for file in Path(SRC_DIR).glob("*"):
        if file.suffix not in [".s", ".S"]:
            continue
        obj_path = Path(BUILD_DIR) / (file.stem + ".o")
        if system == "mac":
            compile_asm_mac(str(file), str(obj_path))
        else:
            raise NotImplementedError("Only macOS (clang) is currently supported.")
        obj_files.append(str(obj_path))

    # 编译 C 并链接
    print("[🔨] Compiling main.c and linking...")
    subprocess.run(["clang", "main.c", *obj_files, "-o", EXECUTABLE], check=True)

    print(f"[✅] Build complete: {EXECUTABLE}")

def run():
    print("[🚀] Running...")
    subprocess.run([EXECUTABLE])

if __name__ == "__main__":
    try:
        build()
        run()
    except subprocess.CalledProcessError as e:
        print(f"[❌] Build failed: {e}")
        sys.exit(1)