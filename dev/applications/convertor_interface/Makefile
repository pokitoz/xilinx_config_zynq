CC=$(CROSS_COMPILE)gcc
CFLAGS=-Wall -std=c99 -O0 -g -Wconversion -Wextra -D APPLICATION_COMPILATION

all: clean demo_convertor clean

demo_convertor: demo_conv.o ../bitmap/bitmap_api.o
	$(CC) -o $@ $^ $(CFLAGS)

../bitmap/bitmap_api.o: ../bitmap/bitmap_api.c ../bitmap/bitmap_api.h
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o ../bitmap/*.o ./generator_yuv/*.o

