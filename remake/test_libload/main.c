#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

// ========== 定义返回值缓冲区 ==========
uint64_t ret_val = 0; // 汇编调用返回的值

// 汇编桥接函数声明
extern void plugin_call_bridge(void *func_ptr, void **args, int argc, uint64_t *ret_ptr);

// 模拟动态库信息
typedef struct {
    const char *lib_name;
    void *handle;
    const char *symbols[16]; // 每个库最多 16 个符号
    void *func_ptrs[16];
    int symbol_count;
} PluginInfo;

#define MAX_PLUGINS 4
PluginInfo plugins[MAX_PLUGINS];
int plugin_count = 0;

// ====== 动态库加载 & 符号扫描 ======
void load_plugin(const char *path, const char *symbols[], int sym_count) {
    if (plugin_count >= MAX_PLUGINS) return;

    void *h = dlopen(path, RTLD_LAZY);
    if (!h) {
        fprintf(stderr, "[ERROR] dlopen: %s\n", dlerror());
        return;
    }

    plugins[plugin_count].lib_name = path;
    plugins[plugin_count].handle = h;
    plugins[plugin_count].symbol_count = sym_count;

    for (int i = 0; i < sym_count; i++) {
        void *fp = dlsym(h, symbols[i]);
        plugins[plugin_count].symbols[i] = symbols[i];
        plugins[plugin_count].func_ptrs[i] = fp;
        printf("[PLUGIN] Loaded symbol: %s -> %p\n", symbols[i], fp);
    }
    plugin_count++;
}

// ====== 模拟 DSL opcode 调度 ======
void dispatch(uint64_t lib_idx, uint64_t func_idx) {
    if (lib_idx >= plugin_count) {
        printf("[DISPATCH] Invalid lib index: %lu\n", lib_idx);
        return;
    }
    if (func_idx >= plugins[lib_idx].symbol_count) {
        printf("[DISPATCH] Invalid func index: %lu\n", func_idx);
        return;
    }

    void *fp = plugins[lib_idx].func_ptrs[func_idx];
    if (!fp) {
        printf("[DISPATCH] Null function pointer!\n");
        return;
    }

    printf("[DISPATCH] Calling %s::%s ...\n",
           plugins[lib_idx].lib_name,
           plugins[lib_idx].symbols[func_idx]);

    // 模拟参数（全部按指针传）
    void *args[2];
    const char *msg = "Hello from VM DSL!";
    args[0] = (void *)msg;
    args[1] = NULL;

    plugin_call_bridge(fp, args, 1, &ret_val);

    printf("[DISPATCH] Return value: %lu\n", ret_val);
}

int main() {
    printf("=== Dynamic Plugin Bridge Test ===\n");

    // 1. 加载测试插件
    const char *syms[] = {
        "test1_int",
        "test2_add",
        "test3_str"
    };
    load_plugin("./libplugin.so", syms, 3);

    // 2. 定义测试 opcode（lib_idx, func_idx）
    uint64_t opcodes[][2] = {
        {0, 0}, // lib0::test1_int
        {0, 1}, // lib0::test2_add
        {0, 2}  // lib0::test3_str
    };

    // 3. 遍历执行
    for (int i = 0; i < 3; i++) {
        dispatch(opcodes[i][0], opcodes[i][1]);
    }

    printf("=== Test Finished ===\n");
    return 0;
}
