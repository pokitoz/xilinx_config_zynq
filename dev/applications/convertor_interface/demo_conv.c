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

#include <string.h>
#include <malloc.h>

#include "../bitmap/bitmap_api.h"
#include "../dma/pl_dma_api_structure.h" 
#include "../includes/xparameters.h"

#define PL_DMA_DRIVER_NAME "/dev/pl_axi_dma_driver"
#define DATA_TRANSFER_LENGTH (640*480)

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
	  if(i % PRINT_NUMBER_BYTE_PER_LINE == 0){
	  	printf("\n");
	  }
  }
  printf("\n");
}


int main(void) {

	bitmap_api_image_test();

	uint8_t* struct_img = (uint8_t*) malloc(DATA_TRANSFER_LENGTH*sizeof(uint8_t));
	memset(struct_img, 0xFF, DATA_TRANSFER_LENGTH);	

	print_buffer(struct_img, 32);

	pl_dma_dev_t dev;
	dev.length_mm2s = DATA_TRANSFER_LENGTH;
	dev.length_s2mm = DATA_TRANSFER_LENGTH;
	dev.base_addr = XPAR_AXI_DMA_0_BASEADDR;
	dev.high_addr = XPAR_AXI_DMA_0_HIGHADDR;
	dev.int_s2mm = XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR;
	dev.int_mm2s = 0;


   	int fd_dma = open_driver(PL_DMA_DRIVER_NAME);
	configure_driver(PL_DMA_DRIVER_NAME, fd_dma, dev);
	read_driver(PL_DMA_DRIVER_NAME, fd_dma, struct_img, DATA_TRANSFER_LENGTH);
	close_driver(PL_DMA_DRIVER_NAME, fd_dma);


	print_buffer(struct_img, 32);
	//void bitmap_api_save(uint8_t* rgb_img, const char* filename, uint32_t height, uint32_t width, uint16_t bitsperpixel);
	bitmap_api_save(struct_img, "output_umage.bmp", 480, 640, 8);





/*

	int fp_mem = open("/dev/mem", O_RDWR | O_SYNC);
	void* virtual_generator_addr = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, XPAR_YUV_GENERATOR_0_S00_AXI_BASEADDR);

	generator_yuv_dev dev =	generator_yuv_inst(virtual_generator_addr);
	generator_yuv_init(&dev, 480, 640, GENERATOR_YUV_MODE_FIXED, 0x3F);

	int row_col = generator_yuv_get_row_col(&dev);
	printf("Row Col : %08x\n", row_col);
	int status = generator_yuv_get_status(&dev);
	printf("Status : %08x\n", status);
	int last_pixel = generator_yuv_get_last_pixel(&dev);
	printf("Last pixel : %08x\n", last_pixel);
	int byte_sent = generator_yuv_get_number_byte_sent(&dev);
	printf("Byte sent : %08x\n", byte_sent);

	generator_yuv_stop(&dev);
	generator_yuv_start(&dev);

	generator_yuv_wait_until_idle(&dev);


	row_col = generator_yuv_get_row_col(&dev);
	printf("Row Col : %08x\n", row_col);
	status = generator_yuv_get_status(&dev);
	printf("Status : %08x\n", status);
	last_pixel = generator_yuv_get_last_pixel(&dev);
	printf("Last pixel : %08x\n", last_pixel);
	byte_sent = generator_yuv_get_number_byte_sent(&dev);
	printf("Byte sent : %08x\n", byte_sent);
*/

	return EXIT_SUCCESS;
}


