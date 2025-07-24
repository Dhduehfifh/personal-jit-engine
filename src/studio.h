#ifndef LIB_STUDIO_H
#define LIB_STUDIO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int lib_studio_init(void);
void lib_studio_cleanup(void);
int lib_studio_switch_library(const char* so_path);
int lib_studio_load_config(const char* config_path);
int lib_studio_register_handler(void* fn_ptr, const char* name);
void* lib_studio_find_handler_by_name(const char* name);
int lib_studio_dispatch_entry(int code, void* ctx);
void lib_studio_init_builtin_dispatch(void);

#ifdef __cplusplus
}
#endif

#endif // LIB_STUDIO_H
