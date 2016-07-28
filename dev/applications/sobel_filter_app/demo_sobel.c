#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

#include <sys/ioctl.h>  /* ioctl */

#include <string.h>
#include <malloc.h>

#include "../bitmap/bitmap_api.h"
#include "../dma/pl_dma_api_structure.h" 
#include "../dma/pl_dma_api.h"
#include "../includes/xparameters.h"
#include "../dma/driver/pl_axi_dma_define.h"

#define PL_DMA_DRIVER_NAME "/dev/pl_axi_dma_driver"

#define IMAGE_COL 640
#define IMAGE_ROW 480
#define IMAGE_SOURCE_RESOLUTION (IMAGE_COL*IMAGE_ROW)
#define IMAGE_BYTES_PER_PIXEL 4

#define DEMO_SOBEL_PRINT_NUMBER_BYTE_PER_LINE 16
int open_driver(const char* driver_name);
void close_driver(const char* driver_name, int fd_driver);
void configure_driver(const char* driver_name, int fd_driver, pl_dma_dev_t pl_dma_dev_input);
void read_driver(const char* driver_name, int fd_driver, uint8_t* structure, uint32_t size);

int open_driver(const char* driver_name) {

    printf(" Open Driver\n");

    int fd_driver = open(driver_name, O_RDWR);
    if (fd_driver == -1) {
        printf("ERROR: could not open \"%s\".\n", driver_name);
        printf("    errno = %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

	return fd_driver;
}

void close_driver(const char* driver_name, int fd_driver) {

    printf(" Close Driver\n");

    int result = close(fd_driver);
    if (result == -1) {
        printf("ERROR: could not close \"%s\".\n", driver_name);
        printf("    errno = %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void configure_driver(const char* driver_name, int fd_driver, pl_dma_dev_t pl_dma_dev_input) {

	printf(" Configure Driver\n");

    int result = write(fd_driver, &pl_dma_dev_input, sizeof(pl_dma_dev_t));
    if (result == -1) {
        printf("ERROR: could not write into \"%s\".\n", driver_name);
        printf("    errno = %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void read_driver(const char* driver_name, int fd_driver, uint8_t* structure, uint32_t size) {

    printf(" Read Driver\n");    

	int result = read(fd_driver, structure, size);
    if (result == -1) {
        printf("ERROR: could not read into \"%s\".\n", driver_name);
        // The error message "Bad address" comes from the error code EFAULT
        // it happens when you pass an address to the kernel which is not a valid virtual address in your process's virtual address space.
        printf("    errno = %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    
}


void print_buffer(void* buffer_address, int size) {
  printf("\n");

  unsigned char* buffer = (unsigned char*) buffer_address;
  int i = 1;
  for (; i <= size; i++) {
      printf("%02x ", buffer[i-1]);
	  if(i % DEMO_SOBEL_PRINT_NUMBER_BYTE_PER_LINE == 0){
	  	printf("\n");
	  }
  }
  printf("\n");
}


int main(void) {

	uint8_t* buffer = (uint8_t*) malloc(IMAGE_SOURCE_RESOLUTION*sizeof(uint8_t));

	const char* y_raw_data_filename = "./yframe420p.y";
	FILE *f = NULL;
	f = fopen(y_raw_data_filename, "rb");
	if (f != NULL){
		int n;
		n = fread(buffer, IMAGE_SOURCE_RESOLUTION, 1, f);
		printf("   Read %d bytes expected %u\n", n, IMAGE_SOURCE_RESOLUTION);
	}
	else{
		printf("ERROR: could not open the file... Abort\n");
		return -1;
	}

	fclose(f);

	uint8_t* buffer_24 = (uint8_t*) malloc(3*IMAGE_SOURCE_RESOLUTION*sizeof(uint8_t));
	bitmap_api_transform_8_to_24(buffer_24, buffer, IMAGE_SOURCE_RESOLUTION);
	bitmap_api_save(buffer_24, "input_sobel_24.bmp", 480, 640, 24);


	//print_buffer(buffer, 32);


	pl_dma_dev_t dev;
	


   	int fd_dma = open_driver(PL_DMA_DRIVER_NAME);
	if (ioctl(fd_dma, PL_AXI_DMA_GET_DEV_INFO, &dev) < 0) {
			perror("Error ioctl PL_AXI_DMA_GET_NUM_DEVICES");
			exit(EXIT_FAILURE);
	}

	printf("app: Before PL_AXI_DMA_DEVICE_CONTROL\n");
	pl_dma_print_desc(dev);

	dev.length_mm2s = IMAGE_SOURCE_RESOLUTION*IMAGE_BYTES_PER_PIXEL;
	dev.length_s2mm = IMAGE_SOURCE_RESOLUTION*IMAGE_BYTES_PER_PIXEL;

	dev.addr_s2mm = DEST_MEM_ADDRESS;
	dev.addr_mm2s = SOURCE_MEM_ADDRESS;

	dev.base_addr = XPAR_AXI_DMA_0_BASEADDR;
	dev.high_addr = XPAR_AXI_DMA_0_HIGHADDR;
	dev.int_s2mm = XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR;
	dev.int_mm2s = XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR;
	//configure_driver(PL_DMA_DRIVER_NAME, fd_dma, dev);

	if (ioctl(fd_dma, PL_AXI_DMA_DEVICE_CONTROL, &dev) < 0) {
		perror("Error ioctl getting device info 1");
		exit(EXIT_FAILURE);
	}

	printf("After PL_AXI_DMA_DEVICE_CONTROL\n");
	pl_dma_print_desc(dev);


	
	if (ioctl(fd_dma, PL_AXI_DMA_PREP_BUF, buffer) < 0) {
		perror("Error ioctl getting device info 2");
		exit(EXIT_FAILURE);
	}

	read_driver(PL_DMA_DRIVER_NAME, fd_dma, buffer, IMAGE_SOURCE_RESOLUTION);

	close_driver(PL_DMA_DRIVER_NAME, fd_dma);
	
	bitmap_api_transform_8_to_24(buffer_24, buffer, IMAGE_SOURCE_RESOLUTION);
	bitmap_api_save(buffer_24, "output_sobel_24.bmp", 480, 640, 24);

	free(buffer);
	free(buffer_24);
	return EXIT_SUCCESS;
}


