CC = gcc
CFLAGS = -Wall -std=c99
TARGET = chmod

# The default rule (make)
all: $(TARGET)

# Rule to compile the program
$(TARGET): chmod.c
	$(CC) $(CFLAGS) -o $(TARGET) chmod.c

# Rule to clean up object files and the executable
clean:
	rm -f $(TARGET)

# Rule to run the program (Example usage)
run: $(TARGET)
	./$(TARGET) 755 file1 file2

.PHONY: all clean run

