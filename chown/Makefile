CC = gcc
CFLAGS = -Wall -std=c99
TARGET = chown

# The default rule (make)
all: $(TARGET)

# Rule to compile the program
$(TARGET): chown.c
	$(CC) $(CFLAGS) -o $(TARGET) chown.c

# Rule to clean up object files and the executable
clean:
	rm -f $(TARGET)

# Rule to run the program (Example usage)
run: $(TARGET)
	./$(TARGET) username:group file1 file2

.PHONY: all clean run

