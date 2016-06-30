#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdint.h>
#include <stdbool.h>
#endif

#include "convertor_yuv_rgb.h"

/*******************************************************************************
 *  Private API
 ******************************************************************************/

/*******************************************************************************
 *  Public API
 ******************************************************************************/

/*
 * Constructs a device structure.
 */
convertor_yuv_rgb_dev convertor_yuv_rgb_inst(void *base) {
    convertor_yuv_rgb_dev dev;
    dev.base = base;
    return dev;
}
   

void convertor_yuv_rgb_config(convertor_yuv_rgb_dev *dev, unsigned char alpha_value) {
    io_write32_c(CONVERTOR_YUV_RGB_REGISTER_SET_DEFAULT_SETTINGS(dev->base), 0x00);   
	uint32_t alpha_value_int = alpha_value;
    io_write32_c(CONVERTOR_YUV_RGB_REGISTER_SET_CONCAT(dev->base), alpha_value << 8);   
    io_write32_c(CONVERTOR_YUV_RGB_REGISTER_SET_ENDIANESS(dev->base), 0x01);   
    io_write32_c(CONVERTOR_YUV_RGB_REGISTER_SET_ENABLE_CONVERTION(dev->base), 0x03);   
}

