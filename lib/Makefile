CC=clang
AS=clang
CFLAGS=-fPIC -O2 -target arm64-apple-macos11
ASFLAGS=-target arm64-apple-macos11
LDFLAGS=-dynamiclib -Wl,-exported_symbols_list,export.exp

BUILD_DIR=build
TARGET=$(BUILD_DIR)/libjit.dylib

SRC_C=jitlib.c bridge.c
SRC_S=dispatch_table.s

OBJ_C=$(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_C))
OBJ_S=$(patsubst %.s,$(BUILD_DIR)/%.o,$(SRC_S))
OBJS=$(OBJ_C) $(OBJ_S)

all: $(TARGET)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(BUILD_DIR)
	$(AS) $(ASFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -rf $(BUILD_DIR)