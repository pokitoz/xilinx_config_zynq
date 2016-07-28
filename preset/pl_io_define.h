#ifndef PL_IO_DEFINE_H
#define PL_IO_DEFINE_H


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
		//#define PRINT_CUSTOM(format,arg...) printk(KERNEL_INFO format,##arg)


	#else

		#include <stdio.h>
		#include <stdint.h>

		#define CUSTOM_CAST(type, ptr)       ((type) (ptr))

		#define io_write32_c(dest, src)     (*CUSTOM_CAST(volatile uint32_t *, (dest)) = (src))
		#define io_read32_c(src)            (*CUSTOM_CAST(volatile uint32_t *, (src)))

	//	#define PRINT_CUSTOM ( format,arg...) printf(format,##arg)
		#define PRINT_CUSTOM printf

	#endif


#endif
