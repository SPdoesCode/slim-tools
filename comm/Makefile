CC = gcc
CFLAGS = -Wall -std=c99
TARGET = comm

# The default rule (make)
all: $(TARGET)

# Rule to compile the program
$(TARGET): comm.c
	$(CC) $(CFLAGS) -o $(TARGET) comm.c

# Rule to clean up object files and the executable
clean:
	rm -f $(TARGET)

# Rule to run the program (Example usage)
run: $(TARGET)
	./$(TARGET) file1.txt file2.txt

.PHONY: all clean run

