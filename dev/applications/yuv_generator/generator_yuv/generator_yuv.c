#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#include "generator_yuv.h"

/*******************************************************************************
 *  Private API
 ******************************************************************************/

/*******************************************************************************
 *  Public API
 ******************************************************************************/

/*
 * Constructs a device structure.
 */
generator_yuv_dev generator_yuv_inst(void *base) {
    generator_yuv_dev dev;
    dev.base = base;
    return dev;
}
   

uint32_t generator_yuv_config(generator_yuv_dev *dev) {
    return (GENERATOR_YUV_RD_CONFIG(dev->base));
}


int generator_yuv_get_status(generator_yuv_dev *dev){
    int value = io_read32_c(GENERATOR_YUV_REGISTER_CONFIG_ADDR(dev->base));
    return value;
}

uint8_t generator_yuv_stopped(generator_yuv_dev *dev){
    return (io_read32_c(GENERATOR_YUV_REGISTER_CONFIG_ADDR(dev->base)) & GENERATOR_YUV_COMMAND_STOP);
}


int generator_yuv_get_last_pixel(generator_yuv_dev *dev){
    int value = io_read32_c(GENERATOR_YUV_REGISTER_LAST_PIXEL_ADDR(dev->base));
    return value;
}


int generator_yuv_get_row_col(generator_yuv_dev *dev){
    int value = io_read32_c(GENERATOR_YUV_REGISTER_ROW_COL_ADDR(dev->base));
    return value;
}

int generator_yuv_get_number_byte_sent(generator_yuv_dev *dev){
    int value = io_read32_c( GENERATOR_YUV_REGISTER_NUMBER_BYTE_SENT_ADDR(dev->base));
    return value;
}


void generator_yuv_wait_until_idle(generator_yuv_dev *dev) {
    while(generator_yuv_stopped(dev) != 1);

}

/*
 * Initializes the generator yuv controller.
 */
void generator_yuv_init(generator_yuv_dev *dev, uint16_t row, uint16_t col, unsigned char mode, unsigned char pixel) {
    generator_yuv_stop(dev);
	io_write32_c(GENERATOR_YUV_REGISTER_MODE_ADDR(dev->base), (pixel << 8) + mode);

    io_write32_c(GENERATOR_YUV_REGISTER_LENGTH_ADDR(dev->base), ((uint32_t)(col) << 16) + (uint32_t) row);
	
	dev->col_row = generator_yuv_get_row_col(dev);
	dev->mode=generator_yuv_config(dev);
	dev->byte_sent=generator_yuv_get_number_byte_sent(dev);


}
	
/*
 */
void generator_yuv_start(generator_yuv_dev *dev) {
    io_write32_c(GENERATOR_YUV_REGISTER_START_ADDR(dev->base), GENERATOR_YUV_COMMAND_START); 
}

/*
 */
void generator_yuv_stop(generator_yuv_dev *dev) {
    io_write32_c(GENERATOR_YUV_REGISTER_STOP_ADDR(dev->base), GENERATOR_YUV_COMMAND_STOP);
}
