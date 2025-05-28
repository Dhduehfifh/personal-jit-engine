CC=clang
CFLAGS=-O2 -Wall -target arm64-apple-macos11

all: jit_runtime

jit_runtime: main.c dynlib.c dispatch_stub.s
	$(CC) $(CFLAGS) main.c dynlib.c dispatch_stub.s -o jit_runtime

clean:
	rm -f jit_runtime