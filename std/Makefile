CC = gcc
CFLAGS = -Wall -O3 -fPIC
AS = nasm
ASFLAGS = -f elf64
LDFLAGS = -no-pie -pthread

C_SRCS = main.c dispatch.c dispatch_table.c atomic_operation.c atomic_wrappers.c page_manager.c memory.c
ASM_SRCS = atomic_operation.asm ops_math.asm

C_OBJS = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.asm=_asm.o)
OBJS = $(C_OBJS) $(ASM_OBJS)

TARGET = jit_engine

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_asm.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f *.o *_asm.o $(TARGET)

.PHONY: all clean
