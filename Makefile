CC=cc
CFLAGS=-std=c99
LDFLAGS=-lm
INCLUDE=-I include

SRC?=llist_tests.c
TARGET=out

all: $(TARGET)
	./$<

# $< - dependency of rule
# $@ - target of rule

$(TARGET): src/$(SRC)
	$(CC) $(CFLAGS) $< $(INCLUDE) -o $@ $(LDFLAGS)