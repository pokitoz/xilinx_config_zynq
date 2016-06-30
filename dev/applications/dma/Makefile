CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall -std=c99 -O0 -g -Wconversion -Wextra -D APPLICATION_COMPILATION

all: clean test_dma_transfer clean

test_dma_transfer: test_dma_transfer.o pl_dma_api.o
	$(CC) -o $@ $^

pl_dma_api.o: pl_dma_api.c pl_dma_api.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

