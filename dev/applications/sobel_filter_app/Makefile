CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall -std=c99 -O0 -g -Wconversion -Wextra -D APPLICATION_COMPILATION

all: clean demo_sobel clean

demo_sobel: demo_sobel.o ../bitmap/bitmap_api.o
	$(CC) -o $@ $^ $(CFLAGS)

../bitmap/bitmap_api.o: ../bitmap/bitmap_api.c ../bitmap/bitmap_api.h
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o ../bitmap/*.o

