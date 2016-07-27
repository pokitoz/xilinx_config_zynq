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

#include "./bitmap_api.h"

#define DATA_TRANSFER_LENGTH (640*480)

int main(void) {

	uint8_t buffer[DATA_TRANSFER_LENGTH];
	memset(buffer, 0x00, DATA_TRANSFER_LENGTH);	

	const char* y_raw_data_filename = "./yframe420p.y";
	FILE *f = NULL;
	f = fopen(y_raw_data_filename, "rb");
	if (f != NULL){
		int n;
		n = fread(buffer, DATA_TRANSFER_LENGTH, 1, f);
		printf("Read %d bytes expected %u\n", n, DATA_TRANSFER_LENGTH);
	}
	else{
		printf("ERROR: could not open the file... Abort\n");
		return -1;
	}

	fclose(f);

	uint8_t buffer_24[DATA_TRANSFER_LENGTH*3];
	bitmap_api_transform_8_to_24(buffer_24, buffer, DATA_TRANSFER_LENGTH);
	bitmap_api_save(buffer_24, "input_sobel.bmp", 480, 640, 24);

	return EXIT_SUCCESS;
}


