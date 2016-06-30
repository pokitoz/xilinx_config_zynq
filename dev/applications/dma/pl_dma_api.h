#ifndef __PL_DMA_API_H
#define __PL_DMA_API_H



#if defined(__KERNEL__)

	#include <linux/module.h>
	#include <linux/kernel.h>
	#include <linux/types.h>	
	#include <linux/version.h>	
	#include <linux/printk.h>
	#include <asm/io.h>
	
	#define io_write32_c(dest, src)   iowrite32(src, dest)  
	#define io_read32_c(src)          ioread32(src)  
	
	#define PRINT_CUSTOM printk
//printk(format,##arg)

#else

	#include <stdio.h>
	#include <stdint.h>

	#define CUSTOM_CAST(type, ptr)       ((type) (ptr))

	#define io_write32_c(dest, src)     (*CUSTOM_CAST(volatile uint32_t *, (dest)) = (src))
	#define io_read32_c(src)            (*CUSTOM_CAST(volatile uint32_t *, (src)))

//	#define PRINT_CUSTOM ( format,arg...) printf(format,##arg)
	#define PRINT_CUSTOM printf

#endif


#define PRINT_NUMBER_BYTE_PER_LINE 8

// All registers are from
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



#define MM2S_DMASR_IDLE_BIT 	1
#define MM2S_DMASR_IOC_IRQ_BIT 12

#define S2MM_DMASR_IDLE_BIT 	1
#define S2MM_DMASR_IOC_IRQ_BIT 12


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


typedef struct pl_dma_dev_t{

	void* addr;
	unsigned int length;

	unsigned int base_addr;
	unsigned int high_addr;

	unsigned int int_s2mm;
	unsigned int int_mm2s;

} pl_dma_dev_t;


pl_dma_dev_t pl_dma_init(
	unsigned int length,
	unsigned int base_addr,
	unsigned int high_addr,
	unsigned int int_s2mm,
	unsigned int int_mm2s
);

unsigned int pl_dma_get_reg(unsigned int* dma_address, int offset);
void pl_dma_set_reg(unsigned int* dma_address, int offset, unsigned int value);

void pl_dma_reset(void* dma_address);
void pl_dma_halt(void* dma_address);
void pl_dma_set_addresses(void* dma_address, unsigned int source_addr, unsigned int dest_addr);
void pl_dma_set_length_master(void* dma_address, unsigned int dma_transfer_length);
void pl_dma_set_length_slave(void* dma_address, unsigned int dma_transfer_length);

void pl_dma_start_channel(void* dma_address);
void pl_dma_set_length(void* dma_address, unsigned int dma_transfer_length);
void pl_dma_s2mm_status(unsigned int* dma_address);
void pl_dma_mm2s_status(unsigned int* dma_address);

int pl_dma_mm2s_sync(unsigned int* dma_address);
int pl_dma_s2mm_sync(unsigned int* dma_address);


void pl_dma_print_buffer(void* virtual_address, int size);
void pl_dma_init_buffer(void* virtual_address, int size);

#endif
