#ifndef __GENERATOR_YUV_IO_H__
#define __GENERATOR_YUV_IO_H__


#if defined(__KERNEL__) || defined(MODULE)

	#include <linux/module.h>
	#include <linux/kernel.h>
	#include <linux/types.h>		
	#include <asm/io.h>
	
	#define io_write32_c(dest, src)   iowrite32(dest, src)  
	#define io_read32_c(src)          ioread32(src)  
	
	#define PRINT_CUSTOM(format,arg...) printk(KERNEL_INFO format,##arg)

#else
	#include <stdio.h>
	#include <stdint.h>
	
	#define CUSTOM_CAST(type, ptr)       ((type) (ptr))

	#define io_write32_c(dest, src)     (*CUSTOM_CAST(volatile uint32_t *, (dest)) = (src))
	#define io_read32_c(src)            (*CUSTOM_CAST(volatile uint32_t *, (src)))

	#define PRINT_CUSTOM(format,arg...) printf(format,##arg)

#endif




// Register Map

#define GENERATOR_YUV_MODE_FIXED                                                        0x00
#define GENERATOR_YUV_MODE_INCREMENTAL                                                  0x01
#define GENERATOR_YUV_MODE_PATTERN                                                   	0x02
#define GENERATOR_YUV_MODE_ALTERNATIVE                                                  0x03

#define GENERATOR_YUV_CONFIG_LENGTH_OFFSET                                             (0 * 4) /* RW */
#define GENERATOR_YUV_CONFIG_START_OFFSET                                              (1 * 4) /* WO */
#define GENERATOR_YUV_CONFIG_STOP_OFFSET                                               (2 * 4) /* WO */
#define GENERATOR_YUV_CONFIG_MODE_OFFSET                                               (3 * 4) /* WO */

#define GENERATOR_YUV_READ_CONFIG_OFFSET                                               (0 * 4) /* R */
#define GENERATOR_YUV_READ_PIXEL_VALUE_OFFSET                                          (1 * 4) /* R */
#define GENERATOR_YUV_READ_NUMBER_BYTE_SENT_OFFSET                                     (2 * 4) /* R */
#define GENERATOR_YUV_READ_COL_ROW_OFFSET                                              (3 * 4) /* R */

#define GENERATOR_YUV_REGISTER_LENGTH_ADDR(base)                                ((void *) ((uint8_t *) (base) + GENERATOR_YUV_CONFIG_LENGTH_OFFSET))
#define GENERATOR_YUV_REGISTER_START_ADDR(base)                                 ((void *) ((uint8_t *) (base) + GENERATOR_YUV_CONFIG_START_OFFSET))
#define GENERATOR_YUV_REGISTER_STOP_ADDR(base)                                  ((void *) ((uint8_t *) (base) + GENERATOR_YUV_CONFIG_STOP_OFFSET))
#define GENERATOR_YUV_REGISTER_MODE_ADDR(base)                                  ((void *) ((uint8_t *) (base) + GENERATOR_YUV_CONFIG_MODE_OFFSET))

#define GENERATOR_YUV_REGISTER_CONFIG_ADDR(base)                               ((void *) ((uint8_t *) (base) + GENERATOR_YUV_READ_CONFIG_OFFSET))
#define GENERATOR_YUV_REGISTER_LAST_PIXEL_ADDR(base)                           ((void *) ((uint8_t *) (base) + GENERATOR_YUV_READ_PIXEL_VALUE_OFFSET))
#define GENERATOR_YUV_REGISTER_NUMBER_BYTE_SENT_ADDR(base)                     ((void *) ((uint8_t *) (base) + GENERATOR_YUV_READ_NUMBER_BYTE_SENT_OFFSET ))
#define GENERATOR_YUV_REGISTER_ROW_COL_ADDR(base)                              ((void *) ((uint8_t *) (base) + GENERATOR_YUV_READ_COL_ROW_OFFSET))


#define GENERATOR_YUV_COMMAND_START                                             (0x2)
#define GENERATOR_YUV_COMMAND_STOP                                              (0x1)


#define GENERATOR_YUV_RD_CONFIG(base)                                           generator_yuv_read_word(GENERATOR_YUV_REGISTER_CONFIG_ADDR((base)))
#define GENERATOR_YUV_RD_PIXEL_VALUE(base)                                      generator_yuv_read_word(GENERATOR_YUV_REGISTER_LAST_PIXEL_ADDR((base)))
#define GENERATOR_YUV_RD_NUMBER_BYTE_SENT(base)                                 generator_yuv_read_word(GENERATOR_YUV_REGISTER_NUMBER_BYTE_SENT_ADDR((base)))
#define GENERATOR_YUV_RD_COL_ROW(base)                                          generator_yuv_read_word(GENERATOR_YUV_REGISTER_ROW_COL_ADDR((base)))







typedef struct generator_yuv_dev{
    void         *base;     /* Base address of component */
	uint32_t mode;
	int col_row;
	int byte_sent;
} generator_yuv_dev;

/*******************************************************************************
 *  Public API
 ******************************************************************************/
generator_yuv_dev generator_yuv_inst(void *base);
void generator_yuv_init(generator_yuv_dev *dev, uint16_t row, uint16_t col, unsigned char mode, unsigned char pixel);
void generator_yuv_start(generator_yuv_dev *dev);
void generator_yuv_stop(generator_yuv_dev *dev);

int generator_yuv_get_status(generator_yuv_dev *dev);
int generator_yuv_get_last_pixel(generator_yuv_dev *dev);
int generator_yuv_get_row_col(generator_yuv_dev *dev);
int generator_yuv_get_number_byte_sent(generator_yuv_dev *dev);
uint32_t generator_yuv_config(generator_yuv_dev *dev);
void generator_yuv_wait_until_idle(generator_yuv_dev *dev);

#endif /* __GENERATOR_YUV_IO_H__ */
