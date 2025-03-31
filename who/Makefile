CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
SRC = who.c
OUT = who

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
