#ifndef __PL_AXI_DMA_DEFINE_H__
#define __PL_AXI_DMA_DEFINE_H__


#define RESERVED_BUFFER_PHYS_ADDR      (0x3FA00000)  /* in Compile.sh from kernel mem=1023M boot argument
                                                      * (1024 - 1)*2^20 = 0x3ff00000
                                                      * (1024 - 6)*2^20 = 0x3FA00000
                                                      */
#define BUFFER_LENGTH_1MB		       (1024 * 1024)
#define RESERVED_BUFFER_NUMBER			4
#define RESERVED_BUFFER_LENGTH         (RESERVED_BUFFER_NUMBER * BUFFER_LENGTH_1MB) /* 4 MB */

#define PRINT_WRAP_WIDTH               (80)

#define PL_AXI_DMA_IOCTL_BASE	'W'
#define PL_AXI_DMA_GET_DEV_INFO			_IO(PL_AXI_DMA_IOCTL_BASE, 1)
#define PL_AXI_DMA_DEVICE_CONTROL		_IO(PL_AXI_DMA_IOCTL_BASE, 2)
#define PL_AXI_DMA_PREP_BUF				_IO(PL_AXI_DMA_IOCTL_BASE, 3)

#define PL_AXI_DMA_START_TRANSFER		_IO(PL_AXI_DMA_IOCTL_BASE, 4)
#define PL_AXI_DMA_STOP_TRANSFER		_IO(PL_AXI_DMA_IOCTL_BASE, 5)
#define PL_AXI_DMA_TEST_TRANSFER		_IO(PL_AXI_DMA_IOCTL_BASE, 6)

#endif
