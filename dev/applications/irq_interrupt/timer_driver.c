#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <asm/io.h>
#include "includes/xparameters.h"

MODULE_LICENSE("GPL");

#define DEVICE_NAME "xilinx_pl_timer"

// Need to add interrupt value of the DTB + 32 to have the correct IRQ number
#define IRQ_NUM		(31+32)

// From http://www.xilinx.com/support/documentation/ip_documentation/axi_timer/v2_0/pg079-axi-timer.pdf


//0h TCSR0 Timer 0 Control and Status Register
#define AXI_TIMER_TCSR_OFFSET		0x00
//04h TLR0 Timer 0 Load Register
#define AXI_TIMER_TLR_OFFSET		0x04
// 08h TCR0 Timer 0 Counter Register
#define AXI_TIMER_TCR_OFFSET		0x08


//Bit 8 T0INT Timer 0 Interrupt
//Read:
//   0 = No interrupt has occurred
//   1 = Interrupt has occurred
//Write:
//   0 = No change in state of T0INT
//   1 = Clear T0INT (clear to 0)
#define AXI_TIMER_CSR_INT_OCCURED_MASK	0x00000100
#define AXI_TIMER_CNT	0xFFFF0000


#define AXI_TIMER_CSR_ENABLE_TMR_MASK	0x00000080
#define AXI_TIMER_CSR_ENABLE_INT_MASK	0x00000040
#define AXI_TIMER_CSR_LOAD_MASK			0x00000020
#define AXI_TIMER_CSR_AUTO_RELOAD_MASK	0x00000010


static struct platform_device *pdev;
void* axi_timer_virtual_address;
static int int_cnt;

static irqreturn_t axi_timer_isr(int irq, void*dev_id)		
{      
  unsigned int data;

  // Disable Timer after 10 Interrupts
  int_cnt++;


  data = ioread32(axi_timer_virtual_address + AXI_TIMER_TCR_OFFSET);
  printk(KERN_INFO "axi_timer_isr: %d Interrupt Occurred ! Timer Count = 0x%08X\n", int_cnt, data);
  
  // Clear Interrupt
  data = ioread32(axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
  iowrite32(data | AXI_TIMER_CSR_INT_OCCURED_MASK, axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
  

  
  if (int_cnt > 10)
    {
      printk(KERN_INFO "axi_timer_isr: 10 interrupts have been occurred. Disabling timer");
      data = ioread32(axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
      iowrite32(data & ~(AXI_TIMER_CSR_ENABLE_TMR_MASK), axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
    }
  
  return IRQ_HANDLED;
}

static int __init axi_timer_init(void)  
{
  unsigned int data;
  
  int_cnt = 0;
  printk(KERN_INFO "axi_timer_init: Initialize Module \"%s\"\n", DEVICE_NAME);
   
  if (request_irq(IRQ_NUM, axi_timer_isr, 0, DEVICE_NAME, NULL)) {
    printk(KERN_ERR "axi_timer_init: Cannot register IRQ %d\n", IRQ_NUM);
    return -EIO;
  }
  else {
    printk(KERN_INFO "axi_timer_init: Registered IRQ %d\n", IRQ_NUM);
  }
  
  printk(KERN_INFO "axi_timer_init: Map physical to virtual memory\n");
  axi_timer_virtual_address = ioremap_nocache(XPAR_AXI_TIMER_0_BASEADDR, XPAR_AXI_TIMER_0_HIGHADDR - XPAR_AXI_TIMER_0_BASEADDR + 1);
  
  printk(KERN_INFO "axi_timer_init: Set Timer count\n");
  iowrite32(AXI_TIMER_CNT, axi_timer_virtual_address + AXI_TIMER_TLR_OFFSET);
  data = ioread32(axi_timer_virtual_address + AXI_TIMER_TLR_OFFSET);
  printk(KERN_INFO "axi_timer_init: Set timer count 0x%08X\n",data);
  

  printk(KERN_INFO "axi_timer_init: Set Timer mode and enable interrupt\n");
  iowrite32(AXI_TIMER_CSR_LOAD_MASK, axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
  iowrite32(AXI_TIMER_CSR_ENABLE_INT_MASK | AXI_TIMER_CSR_AUTO_RELOAD_MASK, axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);

  printk(KERN_INFO "axi_timer_init: Register the module device\n");
  pdev = platform_device_register_simple(DEVICE_NAME, 0, NULL, 0);              
  if (pdev == NULL) {                                                     
    printk(KERN_WARNING "axi_timer_init: Adding platform device \"%s\" failed\n", DEVICE_NAME);
    kfree(pdev);                                                             
    return -ENODEV;                                                          
  }
  

  // Return the list of interrupt non handled: should be given as argument to probe_irq_off;
  //unsigned long bitmask = probe_irq_on();
  printk(KERN_INFO "axi_timer_init: Start timer\n");
  data = ioread32(axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
  iowrite32(data | AXI_TIMER_CSR_ENABLE_TMR_MASK, axi_timer_virtual_address + AXI_TIMER_TCSR_OFFSET);
	
  //Return the IRQ number of the last interrupt non handled generated after the probe_irq_on call
  // If non interruption: 0 or -1  	
  //int my_interrupt_id = probe_irq_off(bitmask);

  //printk(KERN_INFO "My interrupt id: %d", my_interrupt_id);
  printk(KERN_INFO "axi_timer_init: axi_timer driver initialized\n");

  return 0;
} 

static void __exit axi_timer_edit(void)  		
{
  // Exit Device Module
  iounmap(axi_timer_virtual_address);
  free_irq(IRQ_NUM, NULL);
  platform_device_unregister(pdev);                                             
  printk(KERN_INFO "axi_timer_edit: Exit Device Module \"%s\".\n", DEVICE_NAME);
}


module_init(axi_timer_init);
module_exit(axi_timer_edit);

MODULE_AUTHOR ("fdepraz");
MODULE_DESCRIPTION("PL AXI Timer Driver implementation");
MODULE_LICENSE("GPL v2");
