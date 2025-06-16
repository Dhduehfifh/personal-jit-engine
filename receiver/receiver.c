#include "receiver.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
static uint8_t* buffer = NULL;

void* get_write_ptr() {
    if (!buffer) buffer = malloc(BUFFER_SIZE);
    return buffer;
}

void commit_write(void* ptr, size_t len) {
    printf("[Receiver] Received %zu bytes:\n", len);
    for (size_t i = 0; i < len; i++) {
        printf(" 0x%02X", ((uint8_t*)ptr)[i]);
        if ((i + 1) % 8 == 0) printf("\n");
    }
    printf("\n--- End ---\n");
}