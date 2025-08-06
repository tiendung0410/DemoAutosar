MODULE := Can

SRC_DIRS := . ../../Config
INC_DIRS := . ../../Config ../../Inc

OUT_DIR := Out

# Tìm tất cả file .c trong các SRC_DIRS + main.c
SRC_FILES := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.c))
SRC_FILES += ../../main.c

OBJ_FILES := $(addprefix $(OUT_DIR)/,$(notdir $(SRC_FILES:.c=.o)))
INC_OPT := $(foreach d,$(INC_DIRS),-I$d)

CC := gcc
CFLAGS := $(INC_OPT)
TARGET := $(OUT_DIR)/$(MODULE)

all: $(OUT_DIR) $(TARGET)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Rule build object từ source
$(OUT_DIR)/%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

vpath %.c $(SRC_DIRS) ../../

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $(OBJ_FILES)

clean:
	rm -f $(OUT_DIR)/*.o $(TARGET)
