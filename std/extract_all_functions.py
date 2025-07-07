import os
import re

SUPPORTED_EXT = ['.c', '.h', '.S', '.asm']
OUTPUT_FILE = 'all_functions_dump.txt'

# 函数名 -> [ (filename, code_block) ]
function_map = {}

# 简单 C 函数匹配：非 static/extern 行开头，后跟函数定义
c_function_pattern = re.compile(r'^\s*[\w\s\*]+?\s+(\w+)\s*\(.*\)\s*\{')

# 汇编标签匹配：如 label:
asm_label_pattern = re.compile(r'^([a-zA-Z_][a-zA-Z0-9_]*):\s*$')
asm_global_pattern = re.compile(r'^\s*global\s+([a-zA-Z_][a-zA-Z0-9_]*)')

def is_supported_file(filename):
    return any(filename.endswith(ext) for ext in SUPPORTED_EXT)

def extract_c_functions(filepath):
    with open(filepath, 'r', errors='ignore') as f:
        lines = f.readlines()

    i = 0
    while i < len(lines):
        line = lines[i]
        match = c_function_pattern.match(line)
        if match:
            func_name = match.group(1)
            func_lines = [line]
            brace_count = line.count('{') - line.count('}')
            i += 1
            while i < len(lines) and brace_count > 0:
                func_lines.append(lines[i])
                brace_count += lines[i].count('{') - lines[i].count('}')
                i += 1
            function_map.setdefault(func_name, []).append((filepath, ''.join(func_lines)))
        else:
            i += 1

def extract_asm_labels(filepath):
    with open(filepath, 'r', errors='ignore') as f:
        lines = f.readlines()

    current_label = None
    label_lines = []
    for line in lines:
        global_match = asm_global_pattern.match(line)
        label_match = asm_label_pattern.match(line)

        if global_match:
            current_label = global_match.group(1)
            label_lines = [line]
        elif label_match:
            if current_label:
                function_map.setdefault(current_label, []).append((filepath, ''.join(label_lines)))
            current_label = label_match.group(1)
            label_lines = [line]
        else:
            if current_label:
                label_lines.append(line)

    if current_label:
        function_map.setdefault(current_label, []).append((filepath, ''.join(label_lines)))

def walk_all_files():
    for root, _, files in os.walk('.'):
        for filename in files:
            fullpath = os.path.join(root, filename)
            if not is_supported_file(fullpath):
                continue
            if fullpath.endswith(('.c', '.h')):
                extract_c_functions(fullpath)
            elif fullpath.endswith(('.S', '.asm')):
                extract_asm_labels(fullpath)

def write_output():
    with open(OUTPUT_FILE, 'w') as out:
        for func in sorted(function_map.keys()):
            out.write(f'\n\n===== FUNCTION: {func} =====\n')
            for (filename, code) in function_map[func]:
                out.write(f'\n--- From file: {filename} ---\n')
                out.write(code)
                if not code.endswith('\n'):
                    out.write('\n')

    print(f'[OK] 输出完成，保存为: {OUTPUT_FILE}')

if __name__ == "__main__":
    print('[INFO] 正在扫描所有源码文件...')
    walk_all_files()
    write_output()
