CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g
LDFLAGS =

TARGET = p1

SRC = main.c \
      cvl/cvl.c \
      netpbm/netpbm.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(TARGET) $(OBJ)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
