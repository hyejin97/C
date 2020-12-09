CC=gcc
CFLAGS=-g

all: optimal_dirty fifo_dirty lru_dirty clock_dirty

optimal_dirty: optimal_dirty.c
	$(CC) $(CFLAGS) -o $@ optimal_dirty.c

fifo_dirty: fifo_dirty.c
	$(CC) $(CFLAGS) -o $@ fifo_dirty.c

lru_dirty: lru_dirty.c
	$(CC) $(CFLAGS) -o $@ lru_dirty.c

clock_dirty : clock_dirty.c
	$(CC) $(CFLAGS) -o $@ clock_dirty.c

 

clean:
	rm optimal_dirty fifo_dirty lru_dirty clock_dirty

