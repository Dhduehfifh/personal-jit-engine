CC = clang
CFLAGS = -target arm64-apple-macos11 -O2

all: test

test: main.o std_object.o dispatch_table.o
	$(CC) $(CFLAGS) -o test main.o std_object.o dispatch_table.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

dispatch_table.o: dispatch_table.c
	$(CC) $(CFLAGS) -c dispatch_table.c

std_object.o: std_object.s
	$(CC) $(CFLAGS) -c std_object.s

clean:
	rm -f *.o test