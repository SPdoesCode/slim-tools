# Makefile for seq.c

CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = 
TARGET = seq

# Source and object files
SRC = seq.c
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
