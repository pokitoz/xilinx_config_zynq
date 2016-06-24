// Application test for the DMA in PL
// Configure the different registers and start the DMA

// MM2S stands for Memory Map to Stream
// S2MM stands for Stream to Memory Map


#include "./includes/xparameters.h"
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>


//http://www.xilinx.com/support/documentation/ip_documentation/axi_dma/v7_1/pg021_axi_dma.pdf

//CDMACR (CDMA Control Register - Offset 00h)
//This register provides software application control of the AXI CDMA.
#define MM2S_DMACR	0x00
#define MM2S_DMACR_I	(MM2S_DMACR>>2)
//CDMASR (CDMA Status Register- Offset 04h)
//This register provides the status for the Memory Map to Stream DMA Channel.
#define MM2S_DMASR	0x04
#define MM2S_DMASR_I	(MM2S_DMASR >> 2)
//SA (CDMA Source Address Register - Offset 18h)
//This register provides the source address for Simple DMA transfers by AXI CDMA. 
#define MM2S_SA	0x18
#define MM2S_SA_I (MM2S_SA>>2)
//MM2S_LENGTH (MM2S DMA Transfer Length Register — Offset 28h) 
//This register provides the number of bytes to read from system memory and transfer to MM2S AXI4-Stream
#define MM2S_LENGTH	0x28
#define MM2S_LENGTH_I	(MM2S_LENGTH>>2)

//S2MM_DMACR (S2MM DMA Control Register – Offset 30h)
//This register provides control for the Stream to Memory Map DMA Channel.
#define S2MM_DMACR	0x30
#define S2MM_DMACR_I	(S2MM_DMACR>>2)
//S2MM_DMASR (S2MM DMA Status Register- Offset 34h)
//This register provides the status for the Stream to Memory Map DMA Channel.
#define S2MM_DMASR	0x34
#define S2MM_DMASR_I	(S2MM_DMASR>>2)
//S2MM_DA (S2MM DMA Destination Address Register- Offset 48h)
//This register provides the Destination Address for writing to system memory for the Stream to Memory Map to DMA transfer
#define S2MM_DA	0x48
#define S2MM_DA_I	(S2MM_DA>>2)
//S2MM_LENGTH (S2MM DMA Buffer Length Register- Offset 58h)
//This register provides the length in bytes of the buffer to write data from the Stream to Memory map DMA transfer.
#define S2MM_LENGTH	0x58
#define S2MM_LENGTH_I (S2MM_LENGTH>>2)

// mem=1018M boot argument -- (1024 - 6)*2^20 = 0x3FA00000
#define RESERVED_BUFFER_PHYS_ADDR    (0x3FA00000)  
#define SOURCE_MEM_ADDRESS  	RESERVED_BUFFER_PHYS_ADDR
#define DEST_MEM_ADDRESS 		(RESERVED_BUFFER_PHYS_ADDR+0x00100000)

#define MM2S_DMASR_IDLE_BIT 	1
#define MM2S_DMASR_IOC_IRQ_BIT 12

#define S2MM_DMASR_IDLE_BIT 	1
#define S2MM_DMASR_IOC_IRQ_BIT 12

#define DATA_TRANSFER_LENGTH 32
#define PAGE_SIZE 65535

#define PRINT_NUMBER_BYTE_PER_LINE 8



#define DMA_HALTED 0x1
#define DMA_IDLE 0x2
//Scatter Gather Engine Included. DMASR.SGIncld = 1 the AXI DMA is configured for Scatter Gather mode
#define DMA_SCATTER_GATHER 0x8
//DMA Internal Error. This error occurs if the buffer length specified in the fetched descriptor is set to 0.
#define DMA_INTERNAL_ERROR 0x10
//DMA Slave Error. This error occurs if the slave read from the Memory Map interface issues a Slave Error
#define DMA_SLAVE_ERROR 0x20
//DMA Decode Error. This error occurs if the address request points to an invalid address
#define DMA_DECODE_ERROR 0x40
//Scatter Gather Internal Error. This error occurs if a descriptor with the Complete bit already set is fetched
#define DMA_SG_INTERNAL_ERROR 0x100
//Scatter Gather Slave Error. This error occurs if the slave read from on the Memory Map interface issues a Slave Error
#define DMA_SG_SLAVE_ERROR 0x200
//Scatter Gather Decode Error. This error occurs if CURDESC_PTR and/or NXTDESC_PTR point to an invalid address.
#define DMA_SG_DECODE_ERROR 0x400
//Interrupt on Complete. When set to 1 for Scatter/Gather Mode indicates an interrupt event was generated on completion of a descriptor
#define DMA_INTERRUPT_COMPLETE 0x1000
//Interrupt on Delay. When set to 1, indicates an interrupt event was generated on delay timer timeout
#define DMA_INTERRUPT_DELAY 0x2000
//Interrupt on Error. When set to 1, indicates an interrupt event was generated on error
#define DMA_INTERRUPT_ERROR 0x4000

#define DMA_MASK_INTERRUPT 0xF001

void dma_print_status(unsigned int status);
unsigned int dma_set_reg(unsigned int* dma_address, int offset, unsigned int value);
unsigned int dma_get_reg(unsigned int* dma_address, int offset);
void dma_reset(void* dma_address);
void dma_halt(void* dma_address);
void dma_set_addresses(void* dma_address);
int dma_mm2s_sync(unsigned int* dma_address);
int dma_s2mm_sync(unsigned int* dma_address);
void dma_s2mm_status(unsigned int* dma_address);
void dma_mm2s_status(unsigned int* dma_address);
void print_buffer(void* virtual_address, int size);
void init_source_buffer(void* virtual_source_address);
void dma_set_length(void* dma_address);


// Set and modify value in the register of the DMA depedning on the offset of the register
unsigned int dma_set_reg(unsigned int* dma_address, int offset, unsigned int value) {
	*(dma_address+offset) = value;
}

unsigned int dma_get_reg(unsigned int* dma_address, int offset) {
    return *(dma_address + offset);
}


void dma_reset(void* dma_address){
	printf(" DMA reset\n");
    dma_set_reg(dma_address, S2MM_DMACR_I, 4);
    dma_set_reg(dma_address, MM2S_DMACR_I, 4);
    
	dma_s2mm_status(dma_address);
    dma_mm2s_status(dma_address);
}

void dma_halt(void* dma_address){
	printf(" DMA halt\n");
    dma_set_reg(dma_address, S2MM_DMACR_I, 0);
    dma_set_reg(dma_address, MM2S_DMACR_I, 0);
    
	dma_s2mm_status(dma_address);
    dma_mm2s_status(dma_address);
}


void dma_set_addresses(void* dma_address){

	printf(" DMA Set destination address %p\n", (void*) DEST_MEM_ADDRESS);
    dma_set_reg(dma_address, S2MM_DA_I, DEST_MEM_ADDRESS);

    printf(" DMA Set source address %p\n", (void*) SOURCE_MEM_ADDRESS);
    dma_set_reg(dma_address, MM2S_SA_I, SOURCE_MEM_ADDRESS);

    dma_s2mm_status(dma_address); 
    dma_mm2s_status(dma_address);

}




int dma_mm2s_sync(unsigned int* dma_address) {
    unsigned int mm2s_status =  dma_get_reg(dma_address, MM2S_DMASR_I);

    while(!(mm2s_status & 1<<MM2S_DMASR_IOC_IRQ_BIT) || !(mm2s_status & 1<<MM2S_DMASR_IDLE_BIT) ){
        dma_s2mm_status(dma_address);
        dma_mm2s_status(dma_address);

        mm2s_status =  dma_get_reg(dma_address, MM2S_DMASR_I);
    }
}

int dma_s2mm_sync(unsigned int* dma_address) {
    unsigned int s2mm_status = dma_get_reg(dma_address, S2MM_DMASR_I);

    while(!(s2mm_status & 1<<S2MM_DMASR_IOC_IRQ_BIT) || !(s2mm_status & 1<<S2MM_DMASR_IDLE_BIT)){
        dma_s2mm_status(dma_address);
        dma_mm2s_status(dma_address);

        s2mm_status = dma_get_reg(dma_address, S2MM_DMASR_I);
    }
}



void dma_print_status(unsigned int status){

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


void dma_start_channel(void* dma_address){

	printf("Starting channel\n");
    dma_set_reg(dma_address, S2MM_DMACR_I, DMA_MASK_INTERRUPT);
    dma_set_reg(dma_address, MM2S_DMACR_I, DMA_MASK_INTERRUPT);

    dma_s2mm_status(dma_address);
    dma_mm2s_status(dma_address);

}


void dma_set_length(void* dma_address){

	printf("Writing transfer length %d\n", DATA_TRANSFER_LENGTH);
    dma_set_reg(dma_address, S2MM_LENGTH_I, DATA_TRANSFER_LENGTH);
    dma_set_reg(dma_address, MM2S_LENGTH_I, DATA_TRANSFER_LENGTH);

    dma_s2mm_status(dma_address);
    dma_mm2s_status(dma_address);

}

void dma_s2mm_status(unsigned int* dma_address) {
    unsigned int status = dma_get_reg(dma_address, S2MM_DMASR_I);
    printf(" S2MM status (0x%08x@0x%02x):", status, S2MM_DMASR);
    dma_print_status(status);
}

void dma_mm2s_status(unsigned int* dma_address) {
    unsigned int status = dma_get_reg(dma_address, MM2S_DMASR_I);
    printf(" MM2S status (0x%08x@0x%02x):", status, MM2S_DMASR);
    dma_print_status(status);
}

void print_buffer(void* dma_address, int size) {
    char *p = dma_address;
    int i = 1;
    printf("\n");
    for (; i <= size; i++) {
        printf("%02x ", p[i]);
		if(i % PRINT_NUMBER_BYTE_PER_LINE == 0){
			printf("\n");
		}		
    }
    printf("\n");
}

void init_source_buffer(void* dma_source_address){

	unsigned int* buffer_src = (unsigned int*) dma_source_address;

    buffer_src[0]= 0x01020304; 
    buffer_src[1]= 0x05060708;
    buffer_src[2]= 0x090a0b0c;
    buffer_src[3]= 0x0d0e0f10;
    buffer_src[4]= 0x11121314; 
    buffer_src[5]= 0x15161718; 
    buffer_src[6]= 0x191a1b1c;
    buffer_src[7]= 0x1d1e1f20;

}


int main() {

    int fp_mem = open("/dev/mem", O_RDWR | O_SYNC);
    void* virtual_sdma_address = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, XPAR_AXI_DMA_0_BASEADDR);
    void* virtual_source_address  = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, SOURCE_MEM_ADDRESS);
    void* virtual_destination_address = mmap(NULL, PAGE_SIZE , PROT_READ | PROT_WRITE, MAP_SHARED, fp_mem, DEST_MEM_ADDRESS);

	init_source_buffer(virtual_source_address);

	printf("Set dest buffer to 0");	
    memset(virtual_destination_address, 0, 32);


    printf("Source\n");
	print_buffer(virtual_source_address, 32);
    printf("Dest\n"); 
	print_buffer(virtual_destination_address, 32);

    dma_reset(virtual_sdma_address);
    dma_halt(virtual_sdma_address);

    dma_set_addresses(virtual_sdma_address);

	dma_start_channel(virtual_sdma_address);
    
	dma_set_length(virtual_sdma_address);

    
    printf("Waiting for synchronization\n");
    dma_mm2s_sync(virtual_sdma_address);
    dma_s2mm_sync(virtual_sdma_address); 

    printf("New Dest\n"); 
	print_buffer(virtual_destination_address, 32);


    dma_s2mm_status(virtual_sdma_address);
    dma_mm2s_status(virtual_sdma_address);

    printf("Done 6 "); 

	return 0;
}
