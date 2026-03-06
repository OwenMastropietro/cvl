SRC = src
INC = include
OBJ = obj
BIN = bin

CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g -I$(INC)
LDFLAGS =
CC      = gcc
SHELL   = bash

TARGET = $(BIN)/cvl_demo.x

# Source Files
SRC_FILES = $(wildcard $(SRC)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC)/%.c=$(OBJ)/%.o)

# Example Program
EXAMPLES = examples
EXAMPLE_SRC = $(EXAMPLES)/demo.c
EXAMPLE_OBJ = $(OBJ)/demo.o

# Tests
TEST_SRCS := $(wildcard tests/*.c)
TEST_OBJS := $(TEST_SRCS:tests/%.c=$(OBJ)/tests/%.o)
TEST_BIN := $(BIN)/cvl_test.x

# Directories
$(OBJ):
	mkdir -p $(OBJ)

$(OBJ)/tests:
	mkdir -p $(OBJ)/tests

$(BIN):
	mkdir -p $(BIN)


all : $(TARGET)

$(TARGET) : $(OBJ_FILES) $(EXAMPLE_OBJ) | $(BIN)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/demo.o: $(EXAMPLES)/demo.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/tests/%.o: tests/%.c | $(OBJ)/tests
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BIN): $(OBJ_FILES) $(TEST_OBJS) | $(BIN)
	$(CC) $^ -o $@

# Run demo
run: $(TARGET)
	./$(TARGET)


test: $(TEST_BIN)
	./$(TEST_BIN)


clean:
	rm -rf $(OBJ) $(BIN)


.PHONY: all clean run
