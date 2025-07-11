#include "dispatch_table.h"
#include "builtin_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- 函数字段绑定的逻辑：计算找零 ---
void calculate_change(void* instance_data) {
    StructInstance* inst = (StructInstance*)instance_data;
    const StructDef* def = inst->def;

    int offset_price = get_field_offset(def, "price");
    int offset_coin = get_field_offset(def, "coin");
    int offset_change = get_field_offset(def, "change");

    float price = *(float*)(inst->data + offset_price);
    float coin = *(float*)(inst->data + offset_coin);
    float change = coin - price;

    *(float*)(inst->data + offset_change) = change;
}

// 全局变量供各 step 共享
static JitContext ctx;
static StructDef* def = NULL;
static StructInstance* inst = NULL;

// === Step 1: 初始化 Dispatch 表与上下文 ===
void step1() {
    init_builtin_dispatch();         // 注册所有 dispatch opcode
    dispatch_entry(0x01, &ctx);      // jit_init
    dispatch_entry(0x02, &ctx);      // jit_alloc_page
}

// === Step 2: 创建结构体定义，添加字段与函数 ===
void step2() {
    def = create_struct_def("VendingMachine", 0);
    add_field_to_struct(def, "product_id", FIELD_INT, 0);
    add_field_to_struct(def, "price", FIELD_RAW, sizeof(float));
    add_field_to_struct(def, "coin", FIELD_RAW, sizeof(float));
    add_field_to_struct(def, "change", FIELD_RAW, sizeof(float));
    add_function_field(def, "calculate", calculate_change);
    finalize_struct(def);
}

// === Step 3: 创建结构体实例，用户输入并写入数据 ===
void step3() {
    inst = create_struct_instance(&ctx, def);

    int product_id;
    float price = 0.0f;
    float coin = 0.0f;

    printf("=== 自动售货机 ===\n");
    printf("1. 水 - 3.5 元\n");
    printf("2. 饮料 - 5.0 元\n");
    printf("3. 咖啡 - 7.5 元\n");

    printf("请输入商品编号（1~3）：");
    scanf("%d", &product_id);

    switch (product_id) {
        case 1: price = 3.5f; break;
        case 2: price = 5.0f; break;
        case 3: price = 7.5f; break;
        default:
            printf("无效编号\n");
            exit(1);
    }

    printf("请投币金额：");
    scanf("%f", &coin);

    *(int*)(inst->data + get_field_offset(def, "product_id")) = product_id;
    *(float*)(inst->data + get_field_offset(def, "price")) = price;
    *(float*)(inst->data + get_field_offset(def, "coin")) = coin;
}

// === Step 4: 调用结构体内部 calculate 函数字段 ===
void step4() {
    call_function_by_name(inst, def, "calculate");
}

// === Step 5: 显示结果并释放资源 ===
void step5() {
    float change = *(float*)(inst->data + get_field_offset(def, "change"));

    if (change < 0) {
        printf("金额不足，交易失败。\n");
    } else {
        printf("交易成功！找零：%.2f 元\n", change);
    }

    destroy_struct_instance(inst);
    destroy_struct_def(def);
    dispatch_entry(0x03, &ctx);  // jit_free
}

// === 主函数 ===
int main() {
    step1();  // 初始化系统
    step2();  // 定义结构
    step3();  // 用户输入
    step4();  // 运算处理
    step5();  // 输出与清理
    return 0;
}
