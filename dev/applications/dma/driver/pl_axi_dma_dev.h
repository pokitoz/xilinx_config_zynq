#ifndef __PL_AXI_DMA_DEV_H__
#define __PL_AXI_DMA_DEV_H__

#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/io.h>
#include <asm/ioctl.h>

#include "../pl_dma_api.h"

#include "pl_axi_dma_define.h"


typedef struct axi_dma_interface_dev_t{
    atomic_t                         available;
    struct semaphore                 sem;
    void*							 bus_vbase;
    struct cdev                      cdev;

	pl_dma_dev_t 				 	 pl_dma_dev;
} axi_dma_interface_dev_t;



int axi_dma_interface_open(struct inode *inode, struct file *filp);
int axi_dma_interface_release(struct inode *inode, struct file *filp);
ssize_t axi_dma_interface_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t axi_dma_interface_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);


#endif /* __PL_AXI_DMA_DEV_H__ */
