# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L
LDFLAGS = -lssl -lcrypto
TARGET = md5sum
SRC = md5sum.c

# Compilation rule
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Clean rule
clean:
	rm -f $(TARGET)

# Rebuild rule
rebuild: clean $(TARGET)
