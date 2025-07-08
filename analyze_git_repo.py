import os
import re

def is_c_source(f):
    return f.endswith(".c") or f.endswith(".h")

def extract_structs_and_functions(root):
    structs = set()
    functions = set()
    struct_pattern = re.compile(r'\btypedef\s+struct\s+(\w+)\b')
    func_pattern = re.compile(r'^\s*[\w\*\s]+\s+(\w+)\s*\([^;{]*\)\s*\{')

    for subdir, _, files in os.walk(root):
        for f in files:
            path = os.path.join(subdir, f)
            if not is_c_source(f):
                continue
            try:
                with open(path, 'r', errors='ignore') as file:
                    lines = file.readlines()
                    for line in lines:
                        s_match = struct_pattern.search(line)
                        if s_match:
                            structs.add(s_match.group(1))
                        f_match = func_pattern.match(line)
                        if f_match:
                            functions.add(f_match.group(1))
            except Exception as e:
                print(f"Skipped {path}: {e}")
    return structs, functions

def write_results(structs, functions):
    with open("structs.txt", "w") as sfile:
        for s in sorted(structs):
            sfile.write(s + "\n")

    with open("functions.txt", "w") as ffile:
        for f in sorted(functions):
            ffile.write(f + "\n")

    print("[OK] 输出完成: structs.txt, functions.txt")

if __name__ == "__main__":
    print("📦 正在分析当前目录下所有 .c/.h 文件...")
    structs, functions = extract_structs_and_functions(".")
    write_results(structs, functions)
