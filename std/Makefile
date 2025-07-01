CC = gcc
CFLAGS = -Wall -O2 -fPIC
OBJS = main.o struct_def.o struct_instance.o memory.o

all: main

main: $(OBJS)
	$(CC) -o main $(OBJS)

main.o: main.c struct_def.h struct_instance.h memory.h
	$(CC) $(CFLAGS) -c main.c

struct_def.o: struct_def.c struct_def.h
	$(CC) $(CFLAGS) -c struct_def.c

struct_instance.o: struct_instance.c struct_instance.h struct_def.h memory.h
	$(CC) $(CFLAGS) -c struct_instance.c

memory.o: memory.c memory.h
	$(CC) $(CFLAGS) -c memory.c

clean:
	rm -f *.o main
