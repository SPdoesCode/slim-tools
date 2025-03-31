CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = head
SRC = head.c

# Compilation rule
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean rule
clean:
	rm -f $(TARGET)
