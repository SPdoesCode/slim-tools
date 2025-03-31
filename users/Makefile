CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = users
SRC = users.c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)
