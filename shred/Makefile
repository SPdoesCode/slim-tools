CC = gcc
CFLAGS = -Wall -O2
TARGET = shred
SRC = shred.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
