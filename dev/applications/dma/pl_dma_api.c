#include "pl_dma_api.h"
#include <stdio.h>

// Set and modify value in the register of the DMA depedning on the offset of the register
void pl_dma_set_reg(unsigned int* dma_address, int offset, unsigned int value) {
	*(dma_address+offset) = value;
}

unsigned int pl_dma_get_reg(unsigned int* dma_address, int offset) {
    return *(dma_address + offset);
}


void pl_dma_reset(void* dma_address){
	printf(" DMA reset\n");
    pl_dma_set_reg(dma_address, S2MM_DMACR_I, 4);
    pl_dma_set_reg(dma_address, MM2S_DMACR_I, 4);
    
	pl_dma_s2mm_status(dma_address);
    pl_dma_mm2s_status(dma_address);
}

void pl_dma_halt(void* dma_address){
	printf(" DMA halt\n");
    pl_dma_set_reg(dma_address, S2MM_DMACR_I, 0);
    pl_dma_set_reg(dma_address, MM2S_DMACR_I, 0);
    
	pl_dma_s2mm_status(dma_address);
    pl_dma_mm2s_status(dma_address);
}


void pl_dma_set_addresses(void* dma_address, unsigned int source_addr, unsigned int dest_addr){

    printf(" DMA Set source address %x\n", source_addr);
    pl_dma_set_reg(dma_address, MM2S_SA_I, source_addr);

	printf(" DMA Set destination address %x\n", dest_addr);
    pl_dma_set_reg(dma_address, S2MM_DA_I, dest_addr);

    pl_dma_s2mm_status(dma_address); 
    pl_dma_mm2s_status(dma_address);

}


int pl_dma_mm2s_sync(unsigned int* dma_address) {
    unsigned int mm2s_status =  pl_dma_get_reg(dma_address, MM2S_DMASR_I);

    while(!(mm2s_status & 1<<MM2S_DMASR_IOC_IRQ_BIT) || !(mm2s_status & 1<<MM2S_DMASR_IDLE_BIT) ){
        pl_dma_s2mm_status(dma_address);
        pl_dma_mm2s_status(dma_address);

        mm2s_status =  pl_dma_get_reg(dma_address, MM2S_DMASR_I);
    }

	return (int) mm2s_status;
}

int pl_dma_s2mm_sync(unsigned int* dma_address) {
    unsigned int s2mm_status = pl_dma_get_reg(dma_address, S2MM_DMASR_I);

    while(!(s2mm_status & 1<<S2MM_DMASR_IOC_IRQ_BIT) || !(s2mm_status & 1<<S2MM_DMASR_IDLE_BIT)){
        pl_dma_s2mm_status(dma_address);
        pl_dma_mm2s_status(dma_address);

        s2mm_status = pl_dma_get_reg(dma_address, S2MM_DMASR_I);
    }

	return (int) s2mm_status;
}



void pl_dma_print_status(unsigned int status){

	if (status & DMA_HALTED){
		printf(" DMA_HALTED\n");
	} else{ 
		printf(" DMA_RUN\n");
	}
    
	if (status & DMA_IDLE){
		printf(" DMA_IDLE\n");
	}
    
	if (status & DMA_SCATTER_GATHER){
		printf(" DMA_SCATTER_GATHER_MODE\n");
	}

    if (status & DMA_INTERNAL_ERROR){
		printf(" DMA_INTERNAL_ERROR\n");
	}
	
    if (status & DMA_SLAVE_ERROR){
		printf(" DMA_SLAVE_ERROR\n");
	}
	
    if (status & DMA_DECODE_ERROR){
		printf(" DMA_DECODE_ERROR\n");
	}

    if (status & DMA_SG_INTERNAL_ERROR){
		printf(" DMA_SG_INTERNAL_ERROR\n");
	}

    if (status & DMA_SG_SLAVE_ERROR){
		printf(" DMA_SG_SLAVE_ERROR\n");
	}

    if (status & DMA_SG_DECODE_ERROR){
		printf(" DMA_SG_DECODE_ERROR\n");
	}

    if (status & DMA_INTERRUPT_COMPLETE){
		printf(" DMA_INTERRUPT_COMPLETE\n");
	}

    if (status & DMA_INTERRUPT_DELAY){
		printf(" DMA_INTERRUPT_DELAY\n");
	}

    if (status & DMA_INTERRUPT_ERROR){
		printf(" DMA_INTERRUPT_ERROR\n");
	}

    printf("\n");

}


void pl_dma_start_channel(void* dma_address){

	printf("Starting channel\n");
    pl_dma_set_reg(dma_address, S2MM_DMACR_I, DMA_MASK_INTERRUPT);
    pl_dma_set_reg(dma_address, MM2S_DMACR_I, DMA_MASK_INTERRUPT);

    pl_dma_s2mm_status(dma_address);
    pl_dma_mm2s_status(dma_address);

}

void pl_dma_set_length_master(void* dma_address, unsigned int dma_transfer_length){
	printf("Writing transfer length %d\n", dma_transfer_length);
    pl_dma_set_reg(dma_address, MM2S_LENGTH_I, dma_transfer_length);
    pl_dma_mm2s_status(dma_address);
}

void pl_dma_set_length_slave(void* dma_address, unsigned int dma_transfer_length){
	printf("Writing transfer length %d\n", dma_transfer_length);
    pl_dma_set_reg(dma_address, S2MM_LENGTH_I, dma_transfer_length);
    pl_dma_s2mm_status(dma_address);
}

void pl_dma_set_length(void* dma_address, unsigned int dma_transfer_length){

	pl_dma_set_length_slave(dma_address, dma_transfer_length);
	pl_dma_set_length_master(dma_address, dma_transfer_length);

}

void pl_dma_s2mm_status(unsigned int* dma_address) {
    unsigned int status = pl_dma_get_reg(dma_address, S2MM_DMASR_I);
    printf(" S2MM status reg :: 0x%08x :: 0x%02x", status, S2MM_DMASR);
    pl_dma_print_status(status);
}

void pl_dma_mm2s_status(unsigned int* dma_address) {
    unsigned int status = pl_dma_get_reg(dma_address, MM2S_DMASR_I);
    printf(" MM2S status reg :: 0x%08x :: 0x%02x", status, MM2S_DMASR);
    pl_dma_print_status(status);
}
