

---


# JIT Execution Core (Lightweight Opcode-Driven Runtime)

> **作者自述**：这是一个为嵌入式系统、自定义编译器或结构体模拟执行环境打造的最小运行时内核。支持 opcode 指令调度、自定义结构体布局、函数热绑定与自定义内存页分配。

---

## 🌐 概述

该项目旨在构建一个**高度可控、结构明确、可嵌入、可热插拔的 JIT 调度器系统**，支持：

- 自定义结构体（含字段、函数、裸数据、联合体）
- Opcode 指令驱动逻辑执行
- 跨页分配执行块（含可执行 mmap 页）
- 支持运行时自定义 hook（通过函数字段）
- 自动 panic 记录与生命周期管理
- 完全自定义内存布局与函数入口（兼容 C 与汇编）

---

## 📦 项目结构说明

```c
// 内核入口函数结构（示例）
extern int* input_entry(uint8_t opcode);
````

调用入口为 `input_entry(opcode)`，外部调用只需传入 opcode，即可执行对应逻辑。所有内部逻辑使用统一 `dispatch_entry()` 调度函数完成。

---

## 🧠 核心概念

### Opcode 调度结构

每一个 opcode 被映射到一个函数指针，对应具体功能（如 `jit_alloc_page`, `create_struct_def` 等）：

```c
void* dispatch_entry(uint8_t opcode, void* ctx);
```

调度表由 `init_builtin_dispatch()` 注册，默认在第一次调用 `start_receive()` 时初始化。

---

### 生命周期管理接口

```c
void start_receive();  // 启动调度器
int end_receive();     // 停止调度器 & 资源清理
```

`start_receive()` 在首次调用时初始化调度表与状态控制，`end_receive()` 会触发 panic 清理并恢复环境。

---

### Struct 系统

系统支持运行时动态构建结构体：

```c
StructDef* create_struct_def(const char* name, int is_union);
void add_field_to_struct(StructDef*, const char* name, FieldType type, int raw_size);
StructInstance* create_struct_instance(JitContext*, const StructDef*);
```

支持字段类型：

* `FIELD_INT`：整型
* `FIELD_BOOL`：布尔型
* `FIELD_PTR`：指针
* `FIELD_RAW`：裸数据（自定义 size）
* `FIELD_FUNCTION`：函数字段，可用于绑定执行函数或热替换

### 函数字段执行

支持函数字段的注册、调用与热替换：

```c
add_function_field(def, "run", my_fn);
call_function_by_name(instance_data, def, "run");
```

---

## ⚙️ 内存与页面说明

通过 `jit_alloc_page()` 分配 4096 字节执行页(演示数据)，支持 `PROT_EXEC` 权限，兼容汇编与热写入 patch。

结构体实例统一在 code\_page 上分配，页内管理由 `JitContext` 控制：

```c
typedef struct {
    void* code_page;
    size_t code_offset;
    size_t code_page_size;
    size_t requested_alloc_size;
} JitContext;
```

---

## 🧩 Hook / 动态 patch 设计（可选）

系统允许用户绑定结构体 hook 函数，可在结构体生命周期调用前后执行自定义逻辑：

```c
typedef void (*StructInitHook)(void*);
typedef void (*StructFreeHook)(void*);

typedef struct {
    const char* type_name;
    StructInitHook on_init;
    StructFreeHook on_free;
} StructHookTemplate;
```

推荐将 hook 注册表静态维护，并允许用户通过 `jit_register_hook()` 接口挂载。

---

## 🚀 DMA / 内存追踪规避（高级）

可选设计允许使用“非线性 offset + 页分散策略”规避 DMA 和页边界扫描：

* 每个结构实例中加入 header（magic/type/size）；
* 字段偏移可添加非线性扰动；
* 指令执行页可延迟激活，通过特殊 `opcode` 激活（如 `0xFF`）。

---

## 💬 TODO & 计划支持

* [ ] `jit_frame_t`：统一封装结构体 / 页管理 / hook 调度器；
* [ ] 自定义 Opcode 协议（支持链式调用，单包多命令）；
* [ ] 结构体镜像机制（支持快照/回滚）；
* [ ] 汇编自定义跳板函数注册接口；
* [ ] 模拟线程/调度页分离调度器。

---

## 🔐 安全与权限提醒

⚠️ 该系统本质上为 JIT 执行器，具有部分自定义汇编注入能力。请仅在受信环境中使用：

* 不建议直接暴露给互联网入口
* 不建议加载不可信 opcode 指令包
* 不建议开启 unsafe page patch 模式除非必要

---

## ✅ 示例调用（示意）

```c
int* ret = input_entry(0x10);  // Create struct def
input_entry(0x11);             // Add field
input_entry(0x12);             // Finalize struct
input_entry(0x15);             // Create instance
input_entry(0x19);             // Call function field
```

---

## 📜 LICENSE

本项目暂未设置 License，请在学习或研究用途下使用。

---

```

---

