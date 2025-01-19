CC=gcc
CFLAGS=-std=c99 -Wall -DDYNARR_DEBUG
INCLUDE=-I include
LDFLAGS=

TARGET=app
OBJ_DIR=obj
SRC_DIR=src

SRCS=$(shell find $(SRC_DIR) -name '*.c')
OBJS=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.obj, $(SRCS))

all: $(TARGET)

run: $(TARGET)
	./$<

clean:
	rm -drf $(OBJ_DIR)
	rm $(TARGET)
	@clear

$(OBJ_DIR)/%.obj: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo Compiling...
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(TARGET): $(OBJS)
	@echo Linking...
	$(CC) $< -o $@ $(LDFLAGS)