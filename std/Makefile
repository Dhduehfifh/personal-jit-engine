# 编译器和标志设置
CC = gcc
CFLAGS = -Wall -O3 -fPIC
AS = nasm
ASFLAGS = -f elf64
LDFLAGS = -no-pie -pthread

# 正确地用空格分隔源文件
SRCS = atomic_operation.c dispatch.c dispatch_table.c main.c

ASM_SRCS = atomic_operation.asm ops_math.asm

# 构造目标文件列表
C_OBJS = $(SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.asm=.o)  # 保持 .o 扩展名即可
OBJS = $(C_OBJS) $(ASM_OBJS)

# 输出文件名
TARGET = jit_engine

# 默认目标
all: $(TARGET)

# 链接
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

# C 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 汇编编译规则
%.o: %.asm
	$(AS) $(ASFLAGS) $< -o $@

# 清理目标
clean:
	rm -f *.o $(TARGET)

.PHONY: all clean
