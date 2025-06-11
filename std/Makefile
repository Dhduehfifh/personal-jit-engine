CC = clang
CFLAGS = -Wall -O3 -fPIC -mmacosx-version-min=12.0
ASFLAGS = -arch arm64

SRCS = main.c dispatch.c memory.c dispatch.S
OBJS = $(addprefix build/,$(notdir $(SRCS:.c=.o)))
OBJS := $(OBJS:.S=.o)

all: uranium_jit

uranium_jit: $(OBJS)
    $(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c
    @mkdir -p build
    $(CC) $(CFLAGS) -c $< -o $@

build/%.o: src/%.S
    @mkdir -p build
    $(CC) $(ASFLAGS) -c $< -o $@

clean:
    rm -rf build uranium_jit