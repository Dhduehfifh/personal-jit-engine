
all: jit_runtime

jit_runtime: main.o dispatch_table.o asm_jump.o
	clang -O2 -target arm64-apple-macos11 -o jit_runtime main.o dispatch_table.o asm_jump.o

main.o: main.c
	clang -O2 -target arm64-apple-macos11 -c main.c

dispatch_table.o: dispatch_table.c
	clang -O2 -target arm64-apple-macos11 -c dispatch_table.c

asm_jump.o: asm_jump.S
	clang -O2 -target arm64-apple-macos11 -c asm_jump.S

clean:
	rm -f *.o jit_runtime