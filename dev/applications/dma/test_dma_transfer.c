// Application test for the DMA in PL
// Configure the different registers and start the DMA

// MM2S stands for Memory Map to Stream
// S2MM stands for Stream to Memory Map

#include "pl_dma_api.h"
#include "includes/xparameters.h"
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// In byte!
#define DATA_TRANSFER_LENGTH 32
#define PAGE_SIZE 65535


// mem=1018M boot argument -- (1024 - 6)*2^20 = 0x3FA00000
#define RESERVED_BUFFER_PHYS_ADDR    (0x3FA00000)
#define SOURCE_MEM_ADDRESS  	RESERVED_BUFFER_PHYS_ADDR
#define DEST_MEM_ADDRESS 		(RESERVED_BUFFER_PHYS_ADDR+0x00100000)





int main(void) {

  int fp_mem = open("/dev/mem", O_RDWR | O_SYNC);
  void* virtual_pldma_address = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, XPAR_AXI_DMA_0_BASEADDR);
  void* virtual_source_address  = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, SOURCE_MEM_ADDRESS);
  void* virtual_destination_address = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, DEST_MEM_ADDRESS);

  pl_dma_init_buffer(virtual_source_address, DATA_TRANSFER_LENGTH);

  printf("Set dest buffer to 0\n");
  memset(virtual_destination_address, 0, DATA_TRANSFER_LENGTH);

  printf("Source\n");
  pl_dma_print_buffer(virtual_source_address, DATA_TRANSFER_LENGTH);
  printf("Dest\n");
  pl_dma_print_buffer(virtual_destination_address, DATA_TRANSFER_LENGTH);

  pl_dma_reset(virtual_pldma_address);
  pl_dma_halt(virtual_pldma_address);

  pl_dma_set_addresses(virtual_pldma_address, SOURCE_MEM_ADDRESS, DEST_MEM_ADDRESS);

  pl_dma_start_channel(virtual_pldma_address);

  pl_dma_set_length(virtual_pldma_address, DATA_TRANSFER_LENGTH);

  printf("Waiting for synchronization\n");
  pl_dma_mm2s_sync(virtual_pldma_address);
  pl_dma_s2mm_sync(virtual_pldma_address);

  printf("New Dest\n");
  pl_dma_print_buffer(virtual_destination_address, DATA_TRANSFER_LENGTH);


  pl_dma_s2mm_status(virtual_pldma_address);
  pl_dma_mm2s_status(virtual_pldma_address);

	return 0;
}
