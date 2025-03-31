CC = gcc
CFLAGS = -Wall -Wextra -std=c11
TARGET = tee
SRC = tee.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
