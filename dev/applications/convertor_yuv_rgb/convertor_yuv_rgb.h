#ifndef __CONVERTOR_YUV_RGB_IO_H__
#define __CONVERTOR_YUV_RGB_IO_H__

#ifdef __nios2_arch__
#include "io.h"

#define convertor_yuv_rgb_write_word(dest, src) (IOWR_32DIRECT((dest), 0, (src)))
#define convertor_yuv_rgb_read_word(src)        (IORD_32DIRECT((src), 0))

#else

#if defined(__KERNEL__) || defined(MODULE)
#include <linux/types.h>
#else
#include <stdint.h>
#endif

#define CONVERTOR_YUV_RGB_REGISTER_CAST(type, ptr)       ((type) (ptr))

#define convertor_yuv_rgb_write_word(dest, src)     (*CONVERTOR_YUV_RGB_REGISTER_CAST(volatile uint32_t *, (dest)) = (src))
#define convertor_yuv_rgb_read_word(src)            (*CONVERTOR_YUV_RGB_REGISTER_CAST(volatile uint32_t *, (src)))

#endif


// Register Map
#define CONVERTOR_YUV_RGB_SET_CONCAT                                                         (0x00 * 4)
#define CONVERTOR_YUV_RGB_SET_ENDIANESS                                                      (0x01 * 4)
#define CONVERTOR_YUV_RGB_SET_ENABLE_CONVERTION                                              (0x02 * 4)
#define CONVERTOR_YUV_RGB_SET_DEFAULT_SETTINGS                                               (0x03 * 4)

#define CONVERTOR_YUV_RGB_REGISTER_SET_CONCAT(base)                                          ((void *) ((uint8_t *) (base) + CONVERTOR_YUV_RGB_SET_CONCAT))
#define CONVERTOR_YUV_RGB_REGISTER_SET_ENDIANESS(base)        		                         ((void *) ((uint8_t *) (base) + CONVERTOR_YUV_RGB_SET_ENDIANESS))
#define CONVERTOR_YUV_RGB_REGISTER_SET_ENABLE_CONVERTION(base)                               ((void *) ((uint8_t *) (base) + CONVERTOR_YUV_RGB_SET_ENABLE_CONVERTION))
#define CONVERTOR_YUV_RGB_REGISTER_SET_DEFAULT_SETTINGS(base)                                ((void *) ((uint8_t *) (base) + CONVERTOR_YUV_RGB_SET_DEFAULT_SETTINGS))




typedef struct convertor_yuv_rgb_dev{
    void         *base;     /* Base address of component */
} convertor_yuv_rgb_dev;

/*******************************************************************************
 *  Public API
 ******************************************************************************/
convertor_yuv_rgb_dev convertor_yuv_rgb_inst(void *base);
void convertor_yuv_rgb_config(convertor_yuv_rgb_dev *dev, unsigned char alpha_value);


#endif /* __CONVERTOR_YUV_RGB_IO_H__ */
