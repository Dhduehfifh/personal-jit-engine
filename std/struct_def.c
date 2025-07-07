// struct_def.c
#include "struct_def.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* safe_strdup(const char* s) {
    if (!s) return NULL;
    size_t len = strlen(s);
    char* copy = (char*)malloc(len + 1);
    if (copy) memcpy(copy, s, len + 1);
    return copy;
}

StructDef* create_struct_def(const char* name, int is_union) {
    StructDef* def = (StructDef*)malloc(sizeof(StructDef));
    if (!def) return NULL;
    def->name = safe_strdup(name);
    def->is_union = is_union;
    def->field_count = 0;
    def->total_size = 0;
    def->fields = NULL;
    return def;
}

void add_field_to_struct(StructDef* def, const char* field_name, FieldType type, int size_if_raw) {
    if (!def || !field_name) return;
    int size = 0;
    switch (type) {
        case FIELD_INT: size = 4; break;
        case FIELD_BOOL: size = 1; break;
        case FIELD_PTR: size = 8; break;
        case FIELD_RAW: size = size_if_raw; break;
        default: return;
    }

    StructField field;
    field.name = safe_strdup(field_name);
    field.type = type;
    field.size = size;
    if (def->is_union) {
        field.offset = 0;
    } else {
        int last_offset = 0;
        if (def->field_count > 0) {
            StructField* last = &def->fields[def->field_count - 1];
            last_offset = last->offset + last->size;
        }
        field.offset = last_offset;
    }

    def->fields = realloc(def->fields, sizeof(StructField) * (def->field_count + 1));
    def->fields[def->field_count] = field;
    def->field_count++;
}

void finalize_struct(StructDef* def) {
    if (!def) return;
    int max_end = 0;
    for (int i = 0; i < def->field_count; ++i) {
        int end = def->fields[i].offset + def->fields[i].size;
        if (end > max_end) max_end = end;
    }
    def->total_size = max_end;
}

int get_field_offset(const StructDef* def, const char* field_name) {
    if (!def || !field_name) return -1;
    for (int i = 0; i < def->field_count; ++i) {
        if (strcmp(def->fields[i].name, field_name) == 0) {
            return def->fields[i].offset;
        }
    }
    return -1;
}

void destroy_struct_def(StructDef* def) {
    if (!def) return;
    for (int i = 0; i < def->field_count; ++i) {
        free(def->fields[i].name);
    }
    free(def->fields);
    free(def->name);
    free(def);
}
