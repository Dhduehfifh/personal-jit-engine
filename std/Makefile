CC=clang
CFLAGS=-target arm64-apple-macos11 -O2

all: jit_test

jit_test: main.o jit_core.o jit_dispatch.o
	$(CC) -o jit_test main.o jit_core.o jit_dispatch.o

main.o: main.c jit_core.h
	$(CC) $(CFLAGS) -c main.c

jit_core.o: jit_core.c jit_core.h
	$(CC) $(CFLAGS) -c jit_core.c

jit_dispatch.o: jit_dispatch.S
	$(CC) $(CFLAGS) -c jit_dispatch.S

clean:
	rm -f *.o jit_test
