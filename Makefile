CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g
LDFLAGS =

TARGET = p1

SRC = main.c \
      cvl/cvl.c \
      netpbm/netpbm.c

LIB_OBJS = cvl/cvl.o netpbm/netpbm.o

TEST_SRC = $(wildcard tests/*.c)
TEST_OBJS = $(TEST_SRC:.c=.o)
TEST_BIN = test_cvl

all: $(TARGET)

$(TARGET): main.o $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ main.o $(LIB_OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Testing
$(TEST_BIN): $(TEST_OBJS) $(LIB_OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJS) $(LIB_OBJS) $(LDFLAGS)

test: $(TEST_BIN)
	./$(TEST_BIN)


clean:
	rm -f main.o $(TARGET) $(LIB_OBJS) $(TEST_OBJS) $(TEST_BIN)


.PHONY: all clean run
