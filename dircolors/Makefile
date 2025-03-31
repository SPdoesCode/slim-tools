CC = gcc
CFLAGS = -Wall -std=c99
TARGET = dircolors_parser

# The object file and source file
SRC = dircolors.c
OBJ = dircolors.o

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
	./$(TARGET) -c /path/to/dircolors/file

.PHONY: all clean run
