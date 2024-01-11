CC=gcc
CFLAGS=-O3 -std=c11
SRC=main.c
OUT=a.out

.PHONY: all clean

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)