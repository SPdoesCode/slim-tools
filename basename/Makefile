CC = gcc
CFLAGS = -Wall -std=c99
TARGET = basename

# The default rule (make)
all: $(TARGET)

# Rule to compile the program
$(TARGET): basename.c
	$(CC) $(CFLAGS) -o $(TARGET) basename.c

# Rule to clean up object files and the executable
clean:
	rm -f $(TARGET)

# Rule to run the program
run: $(TARGET)
	./$(TARGET) example/path/to/file.txt .txt

.PHONY: all clean run

