# Makefile for sha256sum.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lssl -lcrypto # OpenSSL libraries for SHA-256
TARGET = sha256sum

# Source and object files
SRC = sha256sum.c
OBJ = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Rule to create the target
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

# Rule to compile .c to .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET)

# Ensure the default target is 'all' when typing 'make'
.PHONY: all clean
