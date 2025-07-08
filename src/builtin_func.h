#pragma once
#include <stddef.h>
#include <string.h>
#include <stdint.h>
// necessary struct and enum

//========JitContext:create context to alloc a page========
typedef struct {
    void* code_page;
    size_t code_offset;
    size_t code_page_size;        // ✅ 新增：实际分配大小
    size_t requested_alloc_size;  // ✅ 新增：用户希望分配的大小
} JitContext;
//this kind of struct def is all RAW_FIELDS

//=======Struct def related structs========
typedef enum {
    FIELD_INT, // for INT
    FIELD_BOOL,  // for T/F
    FIELD_PTR,  //for pointer
    FIELD_RAW,  // for pertycular size of raw data, little bit similar to type<T>
    FIELD_FUNCTION  // to put functions
} FieldType;

typedef struct {
    char* name;
    FieldType type;
    int offset; // TODO : write comments
    int size; //only for raw and function ,other struct enum types are fixed size
    void* default_handler; //similar to default open method in windows and macos
} StructField;

typedef struct {
    char* name;
    int is_union;
    int total_size; //suppose to be bite , or should be long (TODO)
    int field_count; //seems to be sth like win64dbg, I forgot what it is 
    StructField* fields;
} StructDef;

typedef struct {
    const StructDef* def;
    void* data;
} StructInstance;
//seems to be just new this 



//========atomic lock struct========
typedef struct {
    volatile int value;
} spinlock_t;


//======== memory alloc , init , and free ========
void jit_init(JitContext* ctx);         //ptr:0x01
void* jit_alloc_page(JitContext* ctx);  //ptr:0x02
void jit_free(JitContext* ctx);         //ptr:0x03
//0x00 is for empty operation ,gonna use in return value

//======== struct def, different type and structure ========
StructDef* create_struct_def(const char* name, int is_union); //need to remove the function call from alloc and free
void add_field_to_struct(StructDef* def, const char* field_name, FieldType type, int size_if_raw);
int get_field_offset(const StructDef* def, const char* field_name);
void finalize_struct(StructDef* def);
void destroy_struct_def(StructDef* def);
//TODO: comment and analyse for this

//======== struct instance ========
StructInstance* create_struct_instance(JitContext* ctx, const StructDef* def);
void destroy_struct_instance(StructInstance* inst); //seems have the same funcction with ptr 0x03, which is jit context free
//this part I dont know which void* to export , expecially to dispatch, there are too much similar functions

//======== function_def: assign, change handeler, activate, find name(offset builted in)
typedef void (*FunctionHandler)(void* ctx);/*QUESTION: what do this do ?*/
void add_function_field(StructDef* def, const char* name, FunctionHandler default_fn);
void update_function_handler(StructField* field, FunctionHandler new_fn);
void call_function_field(void* instance_data, StructField* field);
int call_function_by_name(void* instance_data, const StructDef* def, const char* name);

//========Atomic Operation ========
int atomic_lock(spinlock_t* lock);
void atomic_unlock(spinlock_t* lock);
int atomic_compare_and_swap(int* addr, int expected, int desired);
void atomic_pause();
void asm_buffer_write(char* dst, char value);

//assembly clearify
extern int atomic_lock_asm(int* lock);
extern void atomic_unlock_asm(int* lock);
extern int atomic_cas_asm(int* addr, int expected, int desired);
extern void atomic_pause_asm();

//======== panic ========
//error code define
typedef enum {
    JIT_PANIC_NONE = 0,
    JIT_PANIC_MEM_INIT_FAIL,      // init failed
    JIT_PANIC_OUT_OF_MEMORY,      // running out of memory
    JIT_PANIC_INVALID_FREE,       // illego free
    JIT_PANIC_DOUBLE_FREE,        // free one page multiple times
    JIT_PANIC_ILLEGAL_OPCODE,     // illego command
    JIT_PANIC_STACK_OVERFLOW,     // stack overflow
    JIT_PANIC_GC_CORRUPTION       // gc data broken
} JitPanicCode;

void jit_panic(uint32_t code); //emergency operation
// set self define panic operator
typedef void(*JitPanicHandeler)(uint32_t code);
void set_jit_panic_handler(JitPanicHandeler handler);



//TODO: finish and arrange, double check if there's any more built-in functions -- FINISHED
//TODO: add plug-in ptr& tag, important for GC plugin
//TODO: base math is lost , try to redo(this seems need a new files)
//TODO: switch AT&T assembly to nasm/masm assembly
//REMIND: now is doing the .h file not to the step for .c and .asm/.s(need translate) files --USED
//QUESTION: should I include the page handeler in to this bulitin_func ? or should I put it into dispatch?

