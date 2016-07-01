#include "generator_yuv/generator_yuv.h"
#include "includes/xparameters.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define PAGE_SIZE 65535

int main(void){

	int fp_mem = open("/dev/mem", O_RDWR | O_SYNC);
	void* virtual_generator_addr = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, XPAR_YUV_GENERATOR_0_S00_AXI_BASEADDR);

	generator_yuv_dev dev =	generator_yuv_inst(virtual_generator_addr);
	generator_yuv_init(&dev, 480, 640, GENERATOR_YUV_MODE_FIXED, 0x3F);

	unsigned int row_col = generator_yuv_get_row_col(&dev);
	printf("Row Col : %08x\n", row_col);
	unsigned int status = generator_yuv_get_status(&dev);
	printf("Status : %08x\n", status);
	unsigned int last_pixel = generator_yuv_get_last_pixel(&dev);
	printf("Last pixel : %08x\n", last_pixel);
	unsigned int byte_sent = generator_yuv_get_number_byte_sent(&dev);
	printf("Byte sent : %08x\n", byte_sent);

	generator_yuv_stop(&dev);
	generator_yuv_start(&dev);

	generator_yuv_wait_until_idle(&dev);
	
	return 0;

}
