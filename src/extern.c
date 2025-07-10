#include "dispatch_table.h"
#include "builtin_func.h"
#include <stdint.h>
#include <stdbool.h>

static bool is_started = false;
static int count = 0;

void start_receive() {
    if (!is_started)
    {
        is_started = true;
    }

    if (count == 0)
    {
       init_builtin_dispatch(); 
    }
    /*just for show the logic, I definatlty not gonna use a whole bunch of ifels
    to carry aroud*/
    
}

int end_receive() {
    if (is_started) 
    {
        return -1;
    }
    /*
    here need to put every thing back
    and check if there's anything bad
    */
   jit_panic_cleanup();
   //this function is to clean up&print_out the error&dead zone
   return 0; //normal status

    
}


extern int* input_entry(uint8_t opcode) {
    start_receive();
    JitContext ctx = {
        .code_page = NULL,
        .code_offset = 0,
        .code_page_size = 0,
        .requested_alloc_size = 4096 
        //data up there are just examples
    }; 
    dispatch_entry(opcode, &ctx);
    /*
        this part is to show the logic how to set everything up
        including every execute pages like the bulitin_func and other
        outter lib
        [MAYBE]
        and also ,this part if catch the miss address ptr this gonna consider as data 
        so we can add some un-liner offest into it to prevent it from dma[]
    */
   end_receive();
   return 0;


}