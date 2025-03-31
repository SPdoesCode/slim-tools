CC = gcc
CFLAGS = -Wall -std=c99
TARGET = csplit

# The object file and source file
SRC = csplit.c
OBJ = csplit.o

# Default target to build the program
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to build the object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJ)

# Rule to run the program (example)
run: $(TARGET)
	./$(TARGET) input.txt pattern

.PHONY: all clean run
