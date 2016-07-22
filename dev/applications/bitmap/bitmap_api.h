#ifndef __BITMAP_API_H
#define __BITMAP_APT_H


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



#define _height 600
#define _width 800
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _pixelbytesize _height*_width*_bitsperpixel/8
#define _filesize _pixelbytesize+sizeof(bitmap)
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI
#define pixel 0xFF


#pragma pack(push,1)


typedef struct{
    uint8_t signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct{
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;

typedef struct {
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)


void bitmap_api_save(uint8_t* rgb_img, const char* filename, uint32_t height, uint32_t width, uint16_t bitsperpixel);
void bitmap_api_image_test(void);
void bitmap_api_transform_8_to_24(uint8_t* buffer_24, uint8_t* buffer_8, uint32_t size_buffer_8);
#endif
