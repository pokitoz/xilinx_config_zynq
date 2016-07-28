dma-api-folder := ../dma
bitmap-api-folder := ../bitmap
TARGET = demo_sobel
#
CFLAGS=-Wall -std=c99 -O0 -g -Wconversion -Wextra -D APPLICATION_COMPILATION
LDFLAGS = -g -Wall 
CC = $(CROSS_COMPILE)gcc
ARCH= arm

build: $(TARGET)

all: clean_binary demo_sobel clean

demo_sobel: demo_sobel.o $(bitmap-api-folder)/bitmap_api.o $(dma-api-folder)/pl_dma_api.o
	$(CC) $(LDFLAGS)   $^ -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean_binary:
	rm -f $(TARGET)
	$(MAKE) clean

clean:
	rm -f *.a *.o *~
	rm -f $(bitmap-api-folder)/*.o $(dma-api-folder)/*.o




