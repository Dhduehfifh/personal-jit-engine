# 编译器配置
CC     = gcc
CFLAGS = -O2 -Wall
NASM   = nasm
LDFLAGS = -lm

# 查找所有源文件
C_SRC  := $(wildcard *.c)
ASM_SRC := $(wildcard *.asm)

# 对应的目标文件
C_OBJ  := $(C_SRC:.c=.o)
ASM_OBJ := $(ASM_SRC:.asm=.o)

# 最终可执行文件名
TARGET = test

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(C_OBJ) $(ASM_OBJ)
	$(CC) $(C_OBJ) $(ASM_OBJ) -o $@ $(LDFLAGS)

# .c -> .o 编译规则
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# .asm -> .o 编译规则（NASM）
%.o: %.asm
	$(NASM) -f elf64 $< -o $@

# 清除构建文件
clean:
	rm -f *.o $(TARGET)
