// object_alloc.c
#include "object.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

UObject* create_object_from_mod(JitContext* ctx, UMod* mod) {
    if (!ctx || !mod) {
        fprintf(stderr, "[FATAL] NULL ctx/mod in create_object_from_mod\n");
        return NULL;
    }

    if (!ctx->code_page) {
        fprintf(stderr, "[INFO] Allocating page in create_object\n");
        if (!jit_alloc_page(ctx)) return NULL;
    }

    size_t obj_size = sizeof(UObject) + 128;  // 保守估计最多 128 字节数据
    if (ctx->code_offset + obj_size > ctx->code_page_size) {
        fprintf(stderr, "[ERROR] Not enough memory in page for new object\n");
        return NULL;
    }

    UObject* obj = (UObject*)((char*)ctx->code_page + ctx->code_offset);
    ctx->code_offset += obj_size;

    obj->type_tag = 1;
    obj->ref_count = 1;
    obj->from_mod = mod;
    obj->data = (void*)(obj + 1);

    memset(obj->data, 0, 128);
    return obj;
}

void release_object(UObject* obj) {
    if (!obj) return;
    if (obj->ref_count > 0) obj->ref_count -= 1;
}
