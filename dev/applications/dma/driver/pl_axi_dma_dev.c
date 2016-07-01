#undef __KERNEL__
#define __KERNEL__
#undef MODULE
#define MODULE


#include <linux/cdev.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/fs.h>           /*file structure, open read close */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/semaphore.h>    /* Semaphore */
#include <linux/slab.h>
#include <linux/types.h>
#include <asm/atomic.h>
#include <asm/io.h>
#include <asm/uaccess.h>        /* copy_user */

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>

#include "pl_axi_dma_dev.h"
#include "../includes/xparameters.h"
#include "../pl_dma_api.h"

#define DEVICE_NAME "pl_axi_dma_driver"

 
#define SOURCE_MEM_ADDRESS	RESERVED_BUFFER_PHYS_ADDR
#define DEST_MEM_ADDRESS 	(RESERVED_BUFFER_PHYS_ADDR + BUFFER_LENGTH_1MB)

#define DATA_TRANSFER_LENGTH (32)


static void __exit axi_dma_exit(void);
static int __init axi_dma_init(void);
static void clear_buffer(void);
static int copy_to_user_frame_buffer(char __user *buf, phys_addr_t address , unsigned int length);

static void axi_dma_interface_dev_init(axi_dma_interface_dev_t* driver_dev);
static void axi_dma_interface_dev_end(axi_dma_interface_dev_t* driver_dev);
static int axi_dma_interface_setup_cdev(axi_dma_interface_dev_t* driver_dev);
static irqreturn_t axi_dma_isr_s2mm(int irq, void*dev_id);		
static irqreturn_t axi_dma_isr_mm2s(int irq, void*dev_id);
static int axi_dma_init_interrupt(unsigned int int_s2mm, unsigned int int_mm2s);


struct file_operations axi_dma_fops = {
    .owner   = THIS_MODULE,
    .read    = axi_dma_interface_read,
    .write   = axi_dma_interface_write,
    .open    = axi_dma_interface_open,
    .release = axi_dma_interface_release
};

static struct platform_device *pdev;
axi_dma_interface_dev_t axi_dma_interface_dev;

// store the major number extracted by dev_t
int axi_dma_interface_major = 0;
int axi_dma_interface_minor = 0;

/**
* Clear the part in memory that we are going to write using the DMA
***/
static void clear_buffer(void) {

    uint8_t *virt = ioremap_nocache((phys_addr_t) RESERVED_BUFFER_PHYS_ADDR, RESERVED_BUFFER_LENGTH);
	int k = 0;
	for(k = 0; k < RESERVED_BUFFER_NUMBER; k++){
	
		size_t i = 0;
		uint8_t value = k;
		// Byte to byte initialization
		do {
		    *virt = i;
		    virt += 1;
		    i += 1;
		} while (i < BUFFER_LENGTH_1MB);

	}

    iounmap(virt);
}

static int copy_to_user_frame_buffer(char __user *buf, phys_addr_t address, unsigned int length) {
    int error = 0;
    char *user_buf = buf;

	 void *virt = ioremap_nocache(address, length);            
     if (copy_to_user(user_buf, virt, length)) {
            printk("copy to user fail \n");
            error = -EFAULT;
     }
		
	iounmap(virt);
    
    return error;
}


/*inode reffers to the actual file on disk*/
int axi_dma_interface_open(struct inode *inode, struct file *filp) {
    
	printk(KERN_INFO "axi_dma_interface: axi_dma_interface_open\n");
    axi_dma_interface_dev_t *axi_dma_interface;

    axi_dma_interface = container_of(inode->i_cdev, axi_dma_interface_dev_t, cdev);
    filp->private_data = axi_dma_interface;

    if (!atomic_dec_and_test(&axi_dma_interface->available)) {
        atomic_inc(&axi_dma_interface->available);
        printk(KERN_ALERT "open axi_dma_interface: the device has been opened by some other device, unable to open lock\n");
        return -EBUSY; /* already open */
    }

	return 0;

}

int axi_dma_interface_release(struct inode *inode, struct file *filp) {
    axi_dma_interface_dev_t *axi_dma_interface = filp->private_data;
    atomic_inc(&axi_dma_interface->available); /* release the device */
    return 0;
}

ssize_t axi_dma_interface_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos) {


	// Get the structure of the interface from the device file
    if(filp == NULL){
        printk("axi_dma_interface_read: axi_dma_interface file NULL\n");
		return -ERESTARTSYS;
    }

    axi_dma_interface_dev_t* axi_dma_interface = filp->private_data;
    if(axi_dma_interface == NULL){
        printk("axi_dma_interface_read: axi_dma_interface_dev_t NULL\n");
		return -ERESTARTSYS;
    }

    ssize_t retval = 0;

	if (down_interruptible(&axi_dma_interface->sem)) {
        return -ERESTARTSYS;
    }

    clear_buffer();

////////////////////////////////////////////////////////////////////////////////
	//axi_dma_interface->pl_dma_dev.addr = ioremap_nocache(XPAR_AXI_DMA_0_BASEADDR, XPAR_AXI_DMA_0_HIGHADDR - XPAR_AXI_DMA_0_BASEADDR + 1);
	

	pl_dma_reset(axi_dma_interface->pl_dma_dev.addr);
	pl_dma_halt(axi_dma_interface->pl_dma_dev.addr);

	pl_dma_set_addresses(axi_dma_interface->pl_dma_dev.addr, SOURCE_MEM_ADDRESS, DEST_MEM_ADDRESS);

	pl_dma_start_channel(axi_dma_interface->pl_dma_dev.addr);

	pl_dma_set_length(axi_dma_interface->pl_dma_dev.addr, DATA_TRANSFER_LENGTH);
/*
	pl_dma_mm2s_sync(axi_dma_interface->pl_dma_dev.addr);
	pl_dma_s2mm_sync(axi_dma_interface->pl_dma_dev.addr);
*/
////////////////////////////////////////////////////////////////////////////////
	//iounmap(axi_dma_interface->pl_dma_dev.addr);


    /* start acquisition */ 
    // copy to user  
	

    retval = copy_to_user_frame_buffer(buf, DEST_MEM_ADDRESS, DATA_TRANSFER_LENGTH);
    if (retval) {
        printk("axi_dma_interface_read: Fail copy to user\n");
        goto fail;
    }
fail:
    up(&axi_dma_interface->sem);
    return retval;

}


ssize_t axi_dma_interface_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos) {

	printk(KERN_INFO "axi_dma_interface: axi_dma_interface_write\n");
    axi_dma_interface_dev_t* axi_dma_interface = filp->private_data;
    uint32_t frame_dimensions = 0;
    ssize_t retval = 0;

    if (down_interruptible(&axi_dma_interface->sem)) {
        return -ERESTARTSYS;
    }

    if (copy_from_user(&frame_dimensions, buf, count)) {
        retval = -EFAULT;
        goto fail;
    }
    retval = count;

   
fail:
    up(&axi_dma_interface->sem);
    return retval;

}


static void axi_dma_interface_dev_init(axi_dma_interface_dev_t* driver_dev) {
    
    memset(driver_dev, 0, sizeof(driver_dev));

    atomic_set(&driver_dev->available, 1);
    sema_init(&driver_dev->sem, 1);


	driver_dev->pl_dma_dev = pl_dma_init(DATA_TRANSFER_LENGTH, 
											XPAR_AXI_DMA_0_BASEADDR, 
											XPAR_AXI_DMA_0_HIGHADDR, 
											XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR, 
											XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR
										);

	// Map the physical address of the module to virtual space :)
	//off_t axi_dma_address = (off_t) driver_dev->pl_dma_dev.base_addr;
    //size_t mapping_axi_span = (size_t) (((uintptr_t) driver_dev->pl_dma_dev.high_addr) - ((uintptr_t) driver_dev->pl_dma_dev.base_addr) + 1);    

	driver_dev->pl_dma_dev.addr = ioremap_nocache(XPAR_AXI_DMA_0_BASEADDR, XPAR_AXI_DMA_0_HIGHADDR - XPAR_AXI_DMA_0_BASEADDR + 1);
	//driver_dev->pl_dma_dev.addr = ioremap_nocache(axi_dma_address, mapping_axi_span);

	printk(KERN_INFO "axi_dma_interface_dev_init: ioremap\n");
	
	//axi_dma_init_interrupt(driver_dev->pl_dma_dev.int_s2mm, driver_dev->pl_dma_dev.int_mm2s);

}


static void axi_dma_interface_dev_end(axi_dma_interface_dev_t* driver_dev) {
	iounmap(driver_dev->pl_dma_dev.addr);
	printk(KERN_INFO "axi_dma_interface_dev_end: iounmap\n");
}

static int axi_dma_interface_setup_cdev(axi_dma_interface_dev_t* driver_dev) {
    int error = 0;

	printk(KERN_INFO "axi_dma_interface: axi_dma_interface_setup_cdev\n");
    dev_t devno = MKDEV(axi_dma_interface_major, axi_dma_interface_minor);

    cdev_init(&driver_dev->cdev, &axi_dma_fops);
    driver_dev->cdev.owner = THIS_MODULE;
    driver_dev->cdev.ops = &axi_dma_fops;
    error = cdev_add(&driver_dev->cdev, devno, 1);

    return error;
}



// -------------------------- INTERRUPT (IRQ) ----------
static irqreturn_t axi_dma_isr_s2mm(int irq, void*dev_id)		
{
  printk(KERN_INFO "axi_dma_isr_ss2m: Interrupt!!\n");
  return IRQ_HANDLED;
}

static irqreturn_t axi_dma_isr_mm2s(int irq, void*dev_id)		
{
  printk(KERN_INFO "axi_dma_isr_mm2s: Interrupt!!\n");
  return IRQ_HANDLED;
}

static int axi_dma_init_interrupt(unsigned int int_s2mm, unsigned int int_mm2s){

	if(int_s2mm >= 61 && int_s2mm <= 91){
		if (request_irq(int_s2mm, axi_dma_isr_s2mm, 0, DEVICE_NAME, NULL)) {
			printk(KERN_ERR "axi_dma_init: Cannot register IRQ S2MM%d\n", int_s2mm);
			return -EIO; ///* I/O error */
		}
		else {
			printk(KERN_INFO "axi_dma_init: Registered IRQ SS2M %d\n", int_s2mm);
		}
	}

	if(int_mm2s >= 61 && int_mm2s <= 91){
		if (request_irq(int_mm2s, axi_dma_isr_mm2s, 0, DEVICE_NAME, NULL)) {
			printk(KERN_ERR "axi_dma_init: Cannot register IRQ MM2S%d\n", int_mm2s);
			return -EIO;
		}
		else {
			printk(KERN_INFO "axi_dma_init: Registered IRQ MM2S %d\n", int_mm2s);
		}
	}


	return 0;
}
// -------------------------- END INTERRUPT (IRQ) ----------





static int __init axi_dma_init(void)  
{

    printk(KERN_INFO "axi_dma_interface: axi_dma_init\n");
	dev_t devno = 0;
    int result = 0;

    axi_dma_interface_dev_init(&axi_dma_interface_dev);


    /* register char device */
    /* we will get the major number dynamically this is recommended see book : ldd3*/
    result = alloc_chrdev_region(&devno, axi_dma_interface_minor, 1, DEVICE_NAME);
    axi_dma_interface_major = MAJOR(devno);
    if (result < 0) {
        printk(KERN_WARNING "axi_dma_interface: can't get major number %d\n", axi_dma_interface_major);
        goto fail;
    }

    result = axi_dma_interface_setup_cdev(&axi_dma_interface_dev);
    if (result < 0) {
        printk(KERN_WARNING "axi_dma_interface: error %d adding convertor_interface\n", result);
        goto fail;
    }

    printk(KERN_INFO "axi_dma_interface: module loaded\n");
    return 0;


fail:
    axi_dma_exit();
    return result;
} 


static void __exit axi_dma_exit(void)  		
{

	axi_dma_interface_dev_end(&axi_dma_interface_dev);
  	free_irq(axi_dma_interface_dev.pl_dma_dev.int_s2mm, NULL);
	free_irq(axi_dma_interface_dev.pl_dma_dev.int_mm2s, NULL);
  	platform_device_unregister(pdev);
  	printk(KERN_INFO "axi_dma_interface: axi_dma_exit Exit Device Module \"%s\".\n", DEVICE_NAME);

}

module_init(axi_dma_init);
module_exit(axi_dma_exit);

MODULE_AUTHOR ("fdepraz");
MODULE_DESCRIPTION("PL AXI DMA Driver implementation");
MODULE_LICENSE("GPL");
