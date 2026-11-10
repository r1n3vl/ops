CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Wpedantic -g
SANFLAGS = -fsanitize=address,undefined
TARGET = program
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean run debug

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(SANFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(SANFLAGS) -c $< -o $@

run: all
	./$(TARGET)

debug: all
	gdb ./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
