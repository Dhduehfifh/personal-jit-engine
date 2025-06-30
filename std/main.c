// main.c
#include "mod.h"
#include "object.h"
#include "memory.h"
#include <stdio.h>
#include <string.h>


extern UObject* create_object_entry(JitContext* ctx, UMod* mod);  // 汇编调用入口

int main() {
    printf("[TEST] Uranium OOP System Startup\n");

    JitContext ctx;
    jit_init(&ctx);
    ctx.requested_alloc_size = 4096;

    // 创建一个 MOD
    UMod* mod = create_mod(MOD_OFFSET_STRUCTURAL);
    add_attri_to_mod(mod, "value", ATTRI_INT, 0);

    // 使用汇编入口创建对象
    UObject* obj = create_object_entry(&ctx, mod);

    if (!obj) {
        printf("[ERROR] Failed to create object\n");
        return 1;
    }

    printf("[OK] Object created at %p\n", obj);
    printf("      -> Type Tag: %d\n", obj->type_tag);
    printf("      -> Ref Count: %d\n", obj->ref_count);
    printf("      -> Data Ptr: %p\n", obj->data);

    // 模拟访问（直接将值写入）
    int* int_field = (int*)obj->data;
    *int_field = 114514;
    printf("[SET] int_field = %d\n", *int_field);

    release_object(obj);
    printf("[FREE] Object released\n");

    return 0;
}
