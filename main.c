int add_registers(int a, int b) {
    int result;
    __asm__ (
        "movl %1, %%eax;"
        "addl %2, %%eax;"
        "movl %%eax, %0;"
        : "=r" (result)
        : "r" (a), "r" (b)
        : "%eax"
    );
    return result;
}