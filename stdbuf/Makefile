CC = gcc
CFLAGS = -Wall -O2
TARGET = stdbuf
SRC = stdbuf.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
