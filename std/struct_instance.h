// struct_instance.h
#pragma once
#include "struct_def.h"
#include "memory.h"

typedef struct {
    const StructDef* def;
    void* data;
} StructInstance;

StructInstance* create_struct_instance(JitContext* ctx, const StructDef* def);
void destroy_struct_instance(StructInstance* inst);
