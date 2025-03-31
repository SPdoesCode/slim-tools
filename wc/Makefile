CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = wc.c
OBJ = $(SRC:.c=.o)
TARGET = wc

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
