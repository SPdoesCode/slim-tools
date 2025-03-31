CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = uptime
SRC = uptime.c

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

clean:
	rm -f $(TARGET)
