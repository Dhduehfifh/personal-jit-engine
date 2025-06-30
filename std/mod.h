// mod.h
#pragma once

typedef enum {
    MOD_OFFSET_STRUCTURAL = 0,   // 静态结构：offset 位固定
    MOD_OFFSET_ABSTRACT = 1      // 抽象结构：offset 可跳变
} ModOffsetMode;

typedef enum {
    ATTRI_INT,
    ATTRI_BOOL,
    ATTRI_PTR
} AttriType;

typedef struct {
    char* name;
    AttriType type;
    int offset;
} UAttri;

typedef struct UMod {
    struct UMod* parent_mod;
    ModOffsetMode mode;
    int attri_count;
    UAttri* attri_list;
} UMod;

UMod* create_mod(ModOffsetMode mode);
void add_attri_to_mod(UMod* mod, const char* name, AttriType type, int offset);
void destroy_mod(UMod* mod);
