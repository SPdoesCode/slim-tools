CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = uniq

all: $(TARGET)

$(TARGET): uniq.c
	$(CC) $(CFLAGS) -o $(TARGET) uniq.c

clean:
	rm -f $(TARGET)
