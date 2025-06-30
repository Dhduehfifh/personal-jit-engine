# Makefile for Uranium JIT OOP test
CC = gcc
NASM = nasm
CFLAGS = -Wall -O2 -fPIC
LDFLAGS = 
TARGET = main

SRC_C = \
	memory.c \
	object_alloc.c \
	mod_alloc.c \
	main.c

SRC_ASM = \
	object_entry.asm

OBJ_C = $(SRC_C:.c=.o)
OBJ_ASM = $(SRC_ASM:.asm=.o)

OBJS = $(OBJ_C) $(OBJ_ASM)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(NASM) -f elf64 $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f $(TARGET) *.o
