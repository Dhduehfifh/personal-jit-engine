CC = gcc
CFLAGS = -Wall -O2 -g -fPIC

SRCS = \
    main.c \
    function_def.c \
    struct_def.c \
    struct_instance.c \
    memory.c \
    jit_panic.c

OBJS = $(SRCS:.c=.o)

TARGET = test_main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
