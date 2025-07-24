#include <dlfcn.h>
#include <stdio.h>

int main() {
    void* handle = dlopen("./libjmj_web.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "load error: %s\n", dlerror());
        return 1;
    }

    // 获取函数地址
    int (*start_func)(int) = dlsym(handle, "jmj_web_server_start");
    if (!start_func) {
        fprintf(stderr, "dlsym error: %s\n", dlerror());
        return 1;
    }

    // 启动服务器
    start_func(12345);

    getchar();  // 阻塞，等待测试（你也可以挂 DSL loop）

    dlclose(handle);
    return 0;
}
