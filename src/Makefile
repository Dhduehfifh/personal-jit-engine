# src/Makefile
CC=clang
CFLAGS=-Wall -O2 -target arm64-apple-macos11

all: jit_runtime_test

jit_runtime_test: main.o dispatch_table.o asm_dispatch.o
	$(CC) $(CFLAGS) -o jit_runtime_test main.o dispatch_table.o asm_dispatch.o

main.o: main.c dispatch_table.h
	$(CC) $(CFLAGS) -c main.c

dispatch_table.o: dispatch_table.c dispatch_table.h
	$(CC) $(CFLAGS) -c dispatch_table.c

asm_dispatch.o: asm_dispatch.S
	$(CC) $(CFLAGS) -c asm_dispatch.S

clean:
	rm -f *.o jit_runtime_test