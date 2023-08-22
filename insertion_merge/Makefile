CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11

all: sorting

sorting: main.c insertion_merge.c dual_pivot_quicksort.c
	$(CC) $(CFLAGS) -o main main.c insertion_merge.c dual_pivot_quicksort.c

clean:
	rm -f main
