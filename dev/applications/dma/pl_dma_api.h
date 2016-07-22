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


void pl_dma_reset_s2mm(void* dma_address);
void pl_dma_reset_mm2s(void* dma_address);
void pl_dma_reset(void* dma_address);
void pl_dma_halt_s2mm(void* dma_address);
void pl_dma_halt_mm2s(void* dma_address);
void pl_dma_halt(void* dma_address);
void pl_dma_set_address_mm2s(void* dma_address, unsigned int source_addr);
void pl_dma_set_address_s2mm(void* dma_address, unsigned int dest_addr);
void pl_dma_set_addresses(void* dma_address, unsigned int source_addr, unsigned int dest_addr);
int pl_dma_sync_mm2s(unsigned int* dma_address);
int pl_dma_sync_s2mm(unsigned int* dma_address);
static void pl_dma_print_status(unsigned int status);
void pl_dma_start_channel_s2mm(void* dma_address);
void pl_dma_start_channel_mm2s(void* dma_address);
void pl_dma_start_channel(void* dma_address);
void pl_dma_set_length_mm2s(void* dma_address, unsigned int dma_transfer_length);
void pl_dma_set_length_s2mm(void* dma_address, unsigned int dma_transfer_length);
void pl_dma_set_length(void* dma_address, unsigned int dma_transfer_length);
void pl_dma_status_s2mm(unsigned int* dma_address);
void pl_dma_status_mm2s(unsigned int* dma_address);
void pl_dma_print_buffer(void* buffer_address, int size);
void pl_dma_init_buffer(void* buffer_address, int size);
int pl_dma_is_mm2s_busy(unsigned int* dma_address);
int pl_dma_is_s2mm_busy(unsigned int* dma_address);

#endif
