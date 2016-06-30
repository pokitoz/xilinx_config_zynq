#ifndef __DMA_DRIVER_H__
#define __DMA_DRIVER_H__

#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/io.h>

#include "../pl_dma_api.h"


#define RESERVED_BUFFER_PHYS_ADDR      (0x3FA00000)  /* in Compile.sh from kernel mem=1023M boot argument
                                                      * (1024 - 1)*2^20 = 0x3ff00000
                                                      * (1024 - 6)*2^20 = 0x3FA00000
                                                      */
#define BUFFER_LENGTH_1MB		       (1024 * 1024)
#define RESERVED_BUFFER_NUMBER			4
#define RESERVED_BUFFER_LENGTH         (RESERVED_BUFFER_NUMBER * BUFFER_LENGTH_1MB) /* 4 MB */

#define PRINT_WRAP_WIDTH               (80)

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


#endif /* __DMA_DRIVER_H__ */
