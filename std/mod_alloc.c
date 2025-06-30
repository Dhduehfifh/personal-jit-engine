// mod_alloc.c
#include "mod.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char* safe_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (!copy) {
        fprintf(stderr, "[FATAL] malloc failed in safe_strdup\n");
        return NULL;
    }
    memcpy(copy, s, len + 1);
    return copy;
}

UMod* create_mod(ModOffsetMode mode) {
    UMod* mod = (UMod*)malloc(sizeof(UMod));
    if (!mod) {
        fprintf(stderr, "[FATAL] malloc failed in create_mod\n");
        return NULL;
    }
    mod->parent_mod = NULL;
    mod->mode = mode;
    mod->attri_count = 0;
    mod->attri_list = NULL;
    return mod;
}

void add_attri_to_mod(UMod* mod, const char* name, AttriType type, int offset) {
    if (!mod || !name) {
        fprintf(stderr, "[FATAL] NULL input in add_attri_to_mod\n");
        return;
    }

    mod->attri_list = realloc(mod->attri_list, sizeof(UAttri) * (mod->attri_count + 1));
    if (!mod->attri_list) {
        fprintf(stderr, "[FATAL] realloc failed in add_attri_to_mod\n");
        return;
    }

    UAttri* attr = &mod->attri_list[mod->attri_count];
    attr->name = safe_strdup(name);
    attr->type = type;
    attr->offset = offset;

    mod->attri_count += 1;
}

void destroy_mod(UMod* mod) {
    if (!mod) return;
    for (int i = 0; i < mod->attri_count; ++i) {
        free(mod->attri_list[i].name);
    }
    free(mod->attri_list);
    free(mod);
}
