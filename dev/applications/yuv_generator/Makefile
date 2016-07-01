CC=gcc
CFLAGS=-Wall -std=c99

all: clean appl_generator_yuv clean

appl_generator_yuv: appl_generator_yuv.o generator_yuv.o
	$(CC) -o $@ $^ $(CFLAGS)

generator_yuv.o: generator_yuv/generator_yuv.c generator_yuv/generator_yuv.h
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o ./generator_yuv/*.o

