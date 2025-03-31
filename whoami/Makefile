CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = whoami.c
OUT = whoami

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)
