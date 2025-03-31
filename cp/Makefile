CC = gcc
CFLAGS = -Wall -std=c99
TARGET = cp

# The default rule (make)
all: $(TARGET)

# Rule to compile the program
$(TARGET): cp.c
	$(CC) $(CFLAGS) -o $(TARGET) cp.c

# Rule to clean up object files and the executable
clean:
	rm -f $(TARGET)

# Rule to run the program (Example usage)
run: $(TARGET)
	./$(TARGET) file1.txt file2.txt
	./$(TARGET) -r dir1 dir2

.PHONY: all clean run

