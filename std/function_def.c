// function_def.c
#include "function_def.h"
#include "struct_def.h"
#include <string.h>
#include <stdio.h>

// 添加函数字段
void add_function_field(StructDef* def, const char* name, FunctionHandler default_fn) {
    if (!def || !name) return;

    StructField field;
    field.name = safe_strdup(name);
    field.type = FIELD_FUNCTION;
    field.size = sizeof(void*);
    field.default_handler = (void*)default_fn;

    int last_offset = 0;
    if (def->field_count > 0) {
        StructField* last = &def->fields[def->field_count - 1];
        last_offset = last->offset + last->size;
    }
    field.offset = last_offset;

    def->fields = realloc(def->fields, sizeof(StructField) * (def->field_count + 1));
    def->fields[def->field_count] = field;
    def->field_count++;
}

// 更新 handler
void update_function_handler(StructField* field, FunctionHandler new_fn) {
    if (field && field->type == FIELD_FUNCTION) {
        field->default_handler = (void*)new_fn;
    }
}

// 执行函数（ctx 可为实例数据，也可为外部上下文）
void call_function_field(void* instance_data, StructField* field) {
    if (!field || field->type != FIELD_FUNCTION) {
        fprintf(stderr, "[ERR] Invalid function field\n");
        return;
    }

    FunctionHandler fn = *(FunctionHandler*)((char*)instance_data + field->offset);
    if (fn) {
        fn(instance_data);
    } else if (field->default_handler) {
        ((FunctionHandler)(field->default_handler))(instance_data);
    } else {
        fprintf(stderr, "[WARN] No handler bound to field '%s'\n", field->name);
    }
}

// 查找并调用（封装）
int call_function_by_name(void* instance_data, const StructDef* def, const char* name) {
    if (!def || !name) return -1;
    for (int i = 0; i < def->field_count; ++i) {
        StructField* f = &def->fields[i];
        if (f->type == FIELD_FUNCTION && strcmp(f->name, name) == 0) {
            call_function_field(instance_data, f);
            return 0;
        }
    }
    fprintf(stderr, "[WARN] function '%s' not found\n", name);
    return -1;
}
