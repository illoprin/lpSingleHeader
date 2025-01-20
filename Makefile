CC=clang
CFLAGS=-std=c99 -Wall -fsanitize=address
LDFLAGS=-lm
INCLUDE=-I include

SRC?=llist_tests.c
TARGET=a.out

all: $(TARGET)
	./$<

# $< - dependency of rule
# $@ - target of rule

$(TARGET): src/$(SRC)
	$(CC) $(CFLAGS) $< $(INCLUDE) -o $@ $(LDFLAGS)