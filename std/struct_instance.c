// struct_instance.c
#include "struct_instance.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

StructInstance* create_struct_instance(JitContext* ctx, const StructDef* def) {
    if (!ctx || !def) return NULL;
    if (!ctx->code_page) {
        if (!jit_alloc_page(ctx)) {
            fprintf(stderr, "[FATAL] Failed to alloc page in struct_instance\n");
            return NULL;
        }
    }

    if (ctx->code_offset + def->total_size > ctx->code_page_size) {
        fprintf(stderr, "[ERROR] Page memory full\n");
        return NULL;
    }

    StructInstance* inst = (StructInstance*)malloc(sizeof(StructInstance));
    if (!inst) return NULL;

    inst->data = (void*)((char*)ctx->code_page + ctx->code_offset);
    inst->def = def;
    ctx->code_offset += def->total_size;
    memset(inst->data, 0, def->total_size);
    return inst;
}

void destroy_struct_instance(StructInstance* inst) {
    if (!inst) return;
    free(inst);
}
