#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "includes/xparameters.h"
#include "pl_dma_api.h"

#define DEVICE_NAME "pl_axi_dma"


#define DATA_TRANSFER_LENGTH 32

// mem=1018M boot argument -- (1024 - 6)*2^20 = 0x3FA00000
#define RESERVED_BUFFER_PHYS_ADDR    (0x3FA00000)
#define SOURCE_MEM_ADDRESS  	RESERVED_BUFFER_PHYS_ADDR
#define DEST_MEM_ADDRESS 		(RESERVED_BUFFER_PHYS_ADDR+0x00100000)


static struct platform_device *pdev;
void* axi_dma_virtual_address;
static int int_cnt;

pl_dma_dev dev;



static irqreturn_t axi_dma_isr_s2mm(int irq, void*dev_id)		
{
  printk(KERN_INFO "axi_dma_isr_ss2m: Interrupt!!");
  return IRQ_HANDLED;
}

static irqreturn_t axi_dma_isr_mm2s(int irq, void*dev_id)		
{
  printk(KERN_INFO "axi_dma_isr_mm2s: Interrupt!!");
  return IRQ_HANDLED;
}


static void axi_dma_init_interrupt(unsigned int int_s2mm, unsigned int int_mm2s){

	if(int_s2mm >= 61){
		if (request_irq(IRQ_NUM_S2MM, axi_dma_isr_s2mm, 0, DEVICE_NAME, NULL)) {
			printk(KERN_ERR "axi_dma_init: Cannot register IRQ S2MM%d\n", IRQ_NUM_S2MM);
			return -EIO;
		}
		else {
			printk(KERN_INFO "axi_dma_init: Registered IRQ SS2M %d\n", IRQ_NUM_S2MM);
		}
	}

	if(int_mm2s >= 61){
		if (request_irq(IRQ_NUM_MM2S, axi_dma_isr_mm2s, 0, DEVICE_NAME, NULL)) {
			printk(KERN_ERR "axi_dma_init: Cannot register IRQ MM2S%d\n", IRQ_NUM_MM2S);
			return -EIO;
		}
		else {
			printk(KERN_INFO "axi_dma_init: Registered IRQ MM2S %d\n", IRQ_NUM_MM2S);
		}
	}
}

static int __init axi_dma_init(void)  
{
  unsigned int data;
  
  int_cnt = 0;
  printk(KERN_INFO "axi_dma_init: Initialize Module \"%s\"\n", DEVICE_NAME);
   
  
	printk(KERN_INFO "axi_dma_init: Map physical to virtual memory\n");
	dev = pl_dma_init(0,  XPAR_AXI_DMA_0_BASEADDR, XPAR_AXI_DMA_0_HIGHADDR, XPAR_FABRIC_AXI_DMA_0_S2MM_INTROUT_INTR, XPAR_FABRIC_AXI_DMA_0_MM2S_INTROUT_INTR);
	dev.addr = ioremap_nocache(dev.base_addr, dev.high_addr - dev.base_addr + 1);
  
	axi_dma_init_interrupt(dev.int_s2mm, dev.int_mm2s);
  
	printk(KERN_INFO "axi_dma_init: Register the module device\n");
  	pdev = platform_device_register_simple(DEVICE_NAME, 0, NULL, 0);              
  	if (pdev == NULL) {                                                     
    	printk(KERN_WARNING "axi_dma_init: Adding platform device \"%s\" failed\n", DEVICE_NAME);
    	kfree(pdev);                                                             
    	return -ENODEV;
	}
  
  printk(KERN_INFO "axi_dma_init: axi_dma driver initialized\n");

  return 0;
} 

static void __exit axi_dma_edit(void)  		
{
  	iounmap(axi_dma_virtual_address);
  	free_irq(dev.int_s2mm, NULL);
	free_irq(dev.int_mm2s, NULL);
  	platform_device_unregister(pdev);
  	printk(KERN_INFO "axi_dma_edit: Exit Device Module \"%s\".\n", DEVICE_NAME);
}


module_init(axi_dma_init);
module_exit(axi_dma_edit);

MODULE_AUTHOR ("fdepraz");
MODULE_DESCRIPTION("PL AXI Timer Driver implementation");
MODULE_LICENSE("GPL v2");
