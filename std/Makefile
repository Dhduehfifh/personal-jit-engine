# 编译器
CC = gcc
CFLAGS = -Wall -O3 -fPIC
ASFLAGS =
LDFLAGS = -no-pie

# 源文件
SRCS = main.c dispatch.c memory.c jit_panic.c dispatch_table.c ops_math.asm
OBJS = $(SRCS:.c=.o)
OBJS := $(OBJS:.asm=.o)
OBJS := $(OBJS:.S=.o)

# 输出目标
TARGET = jit_engine

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(ASFLAGS) -c $< -o $@

%.o: %.asm
	nasm -f elf64 $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
