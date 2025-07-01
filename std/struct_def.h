// struct_def.h
#pragma once

typedef enum {
    FIELD_INT,
    FIELD_BOOL,
    FIELD_PTR,
    FIELD_RAW  // 固定大小字节数组
} FieldType;

typedef struct {
    char* name;
    FieldType type;
    int offset;
    int size;  // 仅用于 RAW 类型；其余类型固定
} StructField;

typedef struct {
    char* name;
    int is_union;
    int total_size;
    int field_count;
    StructField* fields;
} StructDef;

StructDef* create_struct_def(const char* name, int is_union);
void add_field_to_struct(StructDef* def, const char* field_name, FieldType type, int size_if_raw);
int get_field_offset(const StructDef* def, const char* field_name);
void finalize_struct(StructDef* def);
void destroy_struct_def(StructDef* def);
