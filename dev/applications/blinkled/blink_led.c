#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stddef.h>


#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE -1)
#define PORTMAP_LED 0x43c00000


void* getAddr(int physical){

	


	void* mapped_dev_base;
	off_t dev_base = physical;


	int memfd = open("/dev/mem", O_RDWR | O_SYNC);
	if(memfd == -1){
		printf("Memory error. Exit\n");
		exit(1);
	}

	

	
	void* mapped_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, memfd, dev_base & ~MAP_MASK);
	if(mapped_base == NULL){
		printf("MMAP error. Exit\n");
		exit(1);
	}

	mapped_dev_base = mapped_base + (dev_base & MAP_MASK);
	return mapped_dev_base;

}

int ctoi( int c )
{
    return c - '0';
}

int main(int argc, char* argv[]){


	int* address = getAddr(PORTMAP_LED);
	//Word alligned
	volatile int* gpio_1 = address;
	volatile int* gpio_2 = address + 1;
	volatile int* gpio_3 = address + 2;

	printf("%d, %d, %d\n", *(gpio_1), *(gpio_2), *(gpio_3));

	int i = 0;
	for(i = 0; i < 10; i++){

		*(gpio_1) = 0x00;
		*(gpio_2) = 0x00;
		*(gpio_3) = 0x00;
	
		printf("%d, %d, %d\n", *(gpio_1), *(gpio_2), *(gpio_3));

		*(gpio_1) = i;
		*(gpio_2) = i+1;
		*(gpio_3) = i+2;
	
		printf("%d, %d, %d\n", *(gpio_1), *(gpio_2), *(gpio_3));
	
	}

	if(argc == 4){
		*(gpio_1) = (int) strtol(argv[1], (char **)NULL, 10);
		*(gpio_2) = (int) strtol(argv[2], (char **)NULL, 10);
		*(gpio_3) = (int) strtol(argv[3], (char **)NULL, 10);
		printf("%d, %d, %d\n", *(gpio_1), *(gpio_2), *(gpio_3));
	}

	return 0;

}
