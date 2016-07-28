#ifndef __PL_DMA_API_H
#define __PL_DMA_API_H





#include "pl_dma_api_structure.h"


void pl_dma_print_desc(pl_dma_dev_t dma_dev);
void pl_dma_print_status(unsigned int status);

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
