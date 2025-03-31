CC = gcc
CFLAGS = -Wall -std=c99
TARGET = chgrp

# The default rule (make)
all: $(TARGET)

# Rule to compile the program
$(TARGET): chgrp.c
	$(CC) $(CFLAGS) -o $(TARGET) chgrp.c

# Rule to clean up object files and the executable
clean:
	rm -f $(TARGET)

# Rule to run the program (Example usage)
run: $(TARGET)
	./$(TARGET) groupname file1 file2

.PHONY: all clean run

