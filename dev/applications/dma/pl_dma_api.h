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

#include "pl_dma_api_structure.h"


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
