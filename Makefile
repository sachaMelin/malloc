CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Wvla
LDFLAGS = -shared
VPATH = src

TARGET_LIB = libmalloc.so
OBJS = malloc.o
BIN = main

all: library

library: $(TARGET_LIB)
$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden -fPIC
$(TARGET_LIB): LDFLAGS += -Wl,--no-undefined
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

check:
	./tests/test_malloc.sh

clean:
	$(RM) $(TARGET_LIB)
	$(RM) $(OBJS)
	$(RM) $(BIN)

.PHONY: all $(TARGET_LIB) clean
