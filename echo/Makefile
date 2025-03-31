# Compiler and flags
CC = gcc
CFLAGS = -Wall -std=c99

# Target executable
TARGET = echo

# Source and object files
SRC = echo.c
OBJ = echo.o

# Default target to build the program
all: $(TARGET)

# Rule to build the target (executable)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Rule to build the object file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

# Clean rule to remove compiled files
clean:
	rm -f $(TARGET) $(OBJ)

# Rule to run the program
run: $(TARGET)
	./$(TARGET) Hello\\nWorld

.PHONY: all clean run
