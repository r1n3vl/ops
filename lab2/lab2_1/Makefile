CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=gnu11
SRC = main.c
TARGET = kitchen

.PHONY: all debug sanitize clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

debug:
	$(CC) $(CFLAGS) -g -O0 $(SRC) -o $(TARGET)

sanitize:
	$(CC) $(CFLAGS) -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
