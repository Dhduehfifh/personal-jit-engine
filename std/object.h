// object.h
#pragma once
#include <stdint.h>
#include "mod.h"
#include "memory.h"

typedef struct {
    uint8_t type_tag;
    uint8_t ref_count;
    void* data;
    UMod* from_mod;
} UObject;



UObject* create_object_from_mod(JitContext* ctx, UMod* mod);
void release_object(UObject* obj);
