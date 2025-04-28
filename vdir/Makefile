CC = gcc
CFLAGS = -Wall -O2
TARGET = vdir
SRC = vdir.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
