#include "bulitin_func.h"

int main() {
    printf("=== Builtin ABI Test Start ===\n");

    empty();

    arr test_ctx;
    alloc(4, &test_ctx);

    test_ctx.data[0] = 123;
    test_ctx.data[1] = 456;

    compare(test_ctx.data[0], test_ctx.data[1]);

    uint64_t jump_target = 42;
    jump_if_eq(&jump_target);
    jump_if_neq(&jump_target);
    jump(&jump_target);

    start_timer(1);
    sleep(1);
    end_timer(1);
    printf("Elapsed time: %lu sec\n", get_elapsed_time(1));
    free_all_timers();

    vm_thread_spawn();
    vm_thread_pause();
    vm_thread_resume();
    vm_thread_kill();
    vm_thread_yield();

    release(test_ctx.size, &test_ctx, NULL);
    termiate(&test_ctx);

    printf("=== Builtin ABI Test Done ===\n");
    return 0;
}
