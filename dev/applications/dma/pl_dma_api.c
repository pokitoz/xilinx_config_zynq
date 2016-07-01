#include "pl_dma_api.h"


static void pl_dma_print_status(unsigned int status);

// Set and modify value in the register of the DMA depedning on the offset of the register
void pl_dma_set_reg(unsigned int* dma_address, int offset, unsigned int value) {
	
	
	io_write32_c(dma_address+offset, value);
//	*(dma_address+offset) = value;
//	iowrite32(dma_address+offset, value);
}

unsigned int pl_dma_get_reg(unsigned int* dma_address, int offset) {
//	return *(dma_address + offset);
//	return (unsigned int) ioread32(dma_address+offset);
	return io_read32_c(dma_address + offset);
}



pl_dma_dev_t pl_dma_init(	unsigned int length,
						unsigned int base_addr,
						unsigned int high_addr,
						unsigned int int_s2mm,
						unsigned int int_mm2s){


	pl_dma_dev_t dev;
	dev.length = length;
	dev.base_addr = base_addr;
	dev.high_addr = high_addr;
	dev.int_s2mm = int_s2mm;
	dev.int_mm2s = int_mm2s;

	return dev;


}


void pl_dma_reset_s2mm(void* dma_address){
	pl_dma_set_reg(dma_address, S2MM_DMACR_I, 4);
	pl_dma_status_s2mm(dma_address);
}

void pl_dma_reset_mm2s(void* dma_address){
	pl_dma_set_reg(dma_address, MM2S_DMACR_I, 4);
	pl_dma_status_mm2s(dma_address);
}


void pl_dma_reset(void* dma_address){
	PRINT_CUSTOM(" DMA reset\n");
	pl_dma_reset_s2mm(dma_address);
	pl_dma_reset_mm2s(dma_address);
}


void pl_dma_halt_s2mm(void* dma_address){
	pl_dma_set_reg(dma_address, S2MM_DMACR_I, 0);
	pl_dma_status_s2mm(dma_address);
}


void pl_dma_halt_mm2s(void* dma_address){
	pl_dma_set_reg(dma_address, MM2S_DMACR_I, 0);
	pl_dma_status_mm2s(dma_address);
}

void pl_dma_halt(void* dma_address){
	PRINT_CUSTOM(" DMA halt\n");
	pl_dma_halt_s2mm(dma_address);
	pl_dma_halt_mm2s(dma_address);
}


void pl_dma_set_address_mm2s(void* dma_address, unsigned int source_addr){
	pl_dma_set_reg(dma_address, MM2S_SA_I, source_addr);
}

void pl_dma_set_address_s2mm(void* dma_address, unsigned int dest_addr){
	pl_dma_set_reg(dma_address, S2MM_DA_I, dest_addr);
}


void pl_dma_set_addresses(void* dma_address, unsigned int source_addr, unsigned int dest_addr){

	PRINT_CUSTOM(" DMA Set source address %x\n", source_addr);
	pl_dma_set_reg(dma_address, MM2S_SA_I, source_addr);

	PRINT_CUSTOM(" DMA Set destination address %x\n", dest_addr);
	pl_dma_set_reg(dma_address, S2MM_DA_I, dest_addr);

}


int pl_dma_sync_mm2s(unsigned int* dma_address) {
	unsigned int mm2s_status =  pl_dma_get_reg(dma_address, MM2S_DMASR_I);

	while(!(mm2s_status & 1<<MM2S_DMASR_IOC_IRQ_BIT) || !(mm2s_status & 1<<MM2S_DMASR_IDLE_BIT) ){
	  pl_dma_status_s2mm(dma_address);
	  pl_dma_status_mm2s(dma_address);

	  mm2s_status =  pl_dma_get_reg(dma_address, MM2S_DMASR_I);
	}

	return (int) mm2s_status;
}

int pl_dma_sync_s2mm(unsigned int* dma_address) {
	unsigned int s2mm_status = pl_dma_get_reg(dma_address, S2MM_DMASR_I);

	while(!(s2mm_status & 1<<S2MM_DMASR_IOC_IRQ_BIT) || !(s2mm_status & 1<<S2MM_DMASR_IDLE_BIT)){
	  pl_dma_status_s2mm(dma_address);
	  pl_dma_status_mm2s(dma_address);

	  s2mm_status = pl_dma_get_reg(dma_address, S2MM_DMASR_I);
	}

	return (int) s2mm_status;
}



static void pl_dma_print_status(unsigned int status){

	if (status & DMA_HALTED){
		PRINT_CUSTOM(" DMA_HALTED\n");
	} else{
		PRINT_CUSTOM(" DMA_RUN\n");
	}

	if (status & DMA_IDLE){
		PRINT_CUSTOM(" DMA_IDLE\n");
	}

	if (status & DMA_SCATTER_GATHER){
		PRINT_CUSTOM(" DMA_SCATTER_GATHER_MODE\n");
	}

	if (status & DMA_INTERNAL_ERROR){
		PRINT_CUSTOM(" DMA_INTERNAL_ERROR\n");
	}

	if (status & DMA_SLAVE_ERROR){
		PRINT_CUSTOM(" DMA_SLAVE_ERROR\n");
	}

	if (status & DMA_DECODE_ERROR){
		PRINT_CUSTOM(" DMA_DECODE_ERROR\n");
	}

	if (status & DMA_SG_INTERNAL_ERROR){
		PRINT_CUSTOM(" DMA_SG_INTERNAL_ERROR\n");
	}

	if (status & DMA_SG_SLAVE_ERROR){
		PRINT_CUSTOM(" DMA_SG_SLAVE_ERROR\n");
	}

	if (status & DMA_SG_DECODE_ERROR){
		PRINT_CUSTOM(" DMA_SG_DECODE_ERROR\n");
	}

	if (status & DMA_INTERRUPT_COMPLETE){
		PRINT_CUSTOM(" DMA_INTERRUPT_COMPLETE\n");
	}

	if (status & DMA_INTERRUPT_DELAY){
		PRINT_CUSTOM(" DMA_INTERRUPT_DELAY\n");
	}

	if (status & DMA_INTERRUPT_ERROR){
		PRINT_CUSTOM(" DMA_INTERRUPT_ERROR\n");
	}

	PRINT_CUSTOM("\n");

}


void pl_dma_start_channel_s2mm(void* dma_address){
	pl_dma_set_reg(dma_address, S2MM_DMACR_I, DMA_MASK_INTERRUPT);
	pl_dma_status_s2mm(dma_address);
}

void pl_dma_start_channel_mm2s(void* dma_address){
	pl_dma_set_reg(dma_address, MM2S_DMACR_I, DMA_MASK_INTERRUPT);
	pl_dma_status_mm2s(dma_address);
}

void pl_dma_start_channel(void* dma_address){

	PRINT_CUSTOM("Starting channel\n");
	pl_dma_start_channel_s2mm(dma_address);
	pl_dma_start_channel_mm2s(dma_address);
	
	pl_dma_status_s2mm(dma_address);
	pl_dma_status_mm2s(dma_address);

}

void pl_dma_set_length_mm2s(void* dma_address, unsigned int dma_transfer_length){
	PRINT_CUSTOM("Writing transfer length %d\n", dma_transfer_length);
	pl_dma_set_reg(dma_address, MM2S_LENGTH_I, dma_transfer_length);
	pl_dma_status_mm2s(dma_address);
}

void pl_dma_set_length_s2mm(void* dma_address, unsigned int dma_transfer_length){
	PRINT_CUSTOM("Writing transfer length %d\n", dma_transfer_length);
	pl_dma_set_reg(dma_address, S2MM_LENGTH_I, dma_transfer_length);
	pl_dma_status_s2mm(dma_address);
}

void pl_dma_set_length(void* dma_address, unsigned int dma_transfer_length){

	pl_dma_set_length_s2mm(dma_address, dma_transfer_length);
	pl_dma_set_length_mm2s(dma_address, dma_transfer_length);

}

void pl_dma_status_s2mm(unsigned int* dma_address) {
	unsigned int status = pl_dma_get_reg(dma_address, S2MM_DMASR_I);
	PRINT_CUSTOM(" S2MM status reg :: 0x%08x :: 0x%02x", status, S2MM_DMASR);
	pl_dma_print_status(status);
}

void pl_dma_status_mm2s(unsigned int* dma_address) {
	unsigned int status = pl_dma_get_reg(dma_address, MM2S_DMASR_I);
	PRINT_CUSTOM(" MM2S status reg :: 0x%08x :: 0x%02x", status, MM2S_DMASR);
	pl_dma_print_status(status);
}



void pl_dma_print_buffer(void* buffer_address, int size) {
  PRINT_CUSTOM("\n");

  unsigned char* buffer = (unsigned char*) buffer_address;
  int i = 1;
  for (; i <= size; i++) {
      PRINT_CUSTOM("%02x ", buffer[i-1]);
  if(i % PRINT_NUMBER_BYTE_PER_LINE == 0){
  	PRINT_CUSTOM("\n");
  }
  }
  PRINT_CUSTOM("\n");
}

void pl_dma_init_buffer(void* buffer_address, int size) {

  unsigned char* buffer = (unsigned char*) buffer_address;
  int i = 0;
  for (; i <= size; i++) {
    buffer[i] = (unsigned char) i;
  }
}

