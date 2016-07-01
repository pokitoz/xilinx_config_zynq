#include "bitmap_api.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>


#define DEFAULT_PIXEL 0xE0

void bitmap_api_image_test(void){
	
	char filename[80] = "out_test.bmp";
	uint32_t height = 480;
	uint32_t width = 640;
	uint16_t byteperpixel = 3;
	uint16_t bitperpixel = (uint16_t) (byteperpixel*8);

	uint8_t* rgb_img = (uint8_t*) malloc(height*width*byteperpixel);
	uint32_t row, col, k;	
	for(row = 0; row < height; row++){
		for(col = 0; col < width*byteperpixel; col = col + byteperpixel){
			for(k = 0; k < byteperpixel; k++){
				rgb_img[row*width*byteperpixel + col + k] = (uint8_t) row;
			}
		}
	}

    bitmap_api_save(rgb_img, filename, height, width, bitperpixel);
	free(rgb_img);

}

static bitmap* bitmap_api_create_bm(uint32_t height, uint32_t width, uint16_t bitsperpixel, uint32_t pixelbytesize, uint32_t filesize){


	bitmap *pbitmap  = (bitmap*) calloc(1,sizeof(bitmap));
    strcpy((char*) pbitmap->fileheader.signature,"BM");
    pbitmap->fileheader.filesize = filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = width;
    pbitmap->bitmapinfoheader.height = height;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;

	return pbitmap;

}

void bitmap_api_save(uint8_t* rgb_img, const char* filename, uint32_t height, uint32_t width, uint16_t bitsperpixel){

	uint32_t pixelbytesize = (height*width*bitsperpixel/8);
	uint32_t filesize = pixelbytesize+sizeof(bitmap);

    bitmap* pbitmap = bitmap_api_create_bm(height, width, bitsperpixel, pixelbytesize, filesize);


	FILE *fp = fopen(filename,"wb");
    fwrite (pbitmap, 1, sizeof(bitmap),fp);
	uint8_t* pixelbuffer = (uint8_t*) malloc(pixelbytesize);
	memset(pixelbuffer, DEFAULT_PIXEL, pixelbytesize);

    fwrite(rgb_img,1, pixelbytesize,fp);
	fwrite(pixelbuffer,1, pixelbytesize,fp);


    fclose(fp);
    free(pbitmap);
    free(pixelbuffer);

}



/*
void bitmap_api_save_superpose(uint8_t * scores){


    FILE *fp = fopen("test.bmp","wb");
    bitmap *pbitmap  = (bitmap*)calloc(1,sizeof(bitmap));
    uint8_t *pixelbuffer = (uint8_t*)malloc((_height)*(_width)*_bitsperpixel/8);
    strcpy((char*) pbitmap->fileheader.signature,"BM");
    pbitmap->fileheader.filesize = _filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize =sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = _width;
    pbitmap->bitmapinfoheader.height = _height;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = _pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter ;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter ;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;

    fwrite (pbitmap, 1, sizeof(bitmap),fp);
	int i = 0;
	int j = 0;

		
	if(supperpose == 0){
		// No supperposition of the image and the scores, fill the bitmap with black pixels
		memset(pixelbuffer, DEFAULT_PIXEL, (_height)*(_width)*_bitsperpixel/8);
	}	else {

//		int j = 0;
//		// Supperposition is enable, fill the bitmap with the image and supperpose the scores on it
//		for(j = 0 ; j < _height*_width; j++){
//			pixelbuffer[3*j+0] = y_img[j] ;
//			pixelbuffer[3*j+1] = y_img[j];
//			pixelbuffer[3*j+2] = y_img[j];
//		}

	}

	int img_stride_2 = _width*3;
	int pixel_tr[16] = {
		0 + img_stride_2 * 3,
		3*1 + img_stride_2 * 3,
		3*2 + img_stride_2 * 2,
		3*3 + img_stride_2 * 1,
		3*3 + img_stride_2 * 0,
		3*3 + img_stride_2 * -1,
		3*2 + img_stride_2 * -2,
		3*1 + img_stride_2 * -3,
		3*0 + img_stride_2 * -3,
		3*-1 + img_stride_2 * -3,
		3*-2 + img_stride_2 * -2,
		3*-3 + img_stride_2 * -1,
		3*-3 + img_stride_2 * 0,
		3*-3 + img_stride_2 * 1,
		3*-2 + img_stride_2 * 2,
		3*-1 + img_stride_2 * 3,
	};




	for(i = 0; i < _height; i++){

		if(i == 0 || i == 1 || i == 2 ){
			for(j = 0; j < _width; j++){
				pixelbuffer[3*i*_width + 3*j + 0] = 0x0;
				pixelbuffer[3*i*_width + 3*j + 1] = 0x0;
				pixelbuffer[3*i*_width + 3*j + 2] = 0x0;
			}

		}else if(i == _height-1 || i == _height-2 || i == _height-3){
			for(j = 0; j < _width; j++){
				pixelbuffer[3*i*_width + 3*j + 0] = 0x0;
				pixelbuffer[3*i*_width + 3*j + 1] = 0x0;
				pixelbuffer[3*i*_width + 3*j + 2] = 0x0;
			}

		}else{

			for(j = 0; j < _width; j+=1){
				if(j == 0 || j == 1 || j ==2 || j == _width-1 || j == _width-2 || j==_width-3 ) {
					pixelbuffer[3*i*_width + 3*j + 0] = 0x0;
					pixelbuffer[3*i*_width + 3*j + 1] = 0x0;
					pixelbuffer[3*i*_width + 3*j + 2] = 0x0;
				}else{
					uint8_t thresh = 0;
					if(scores[(i-3)*(_width-1) + j-3] > thresh ){
						pixelbuffer[3*i*_width + 3*j + 0] = scores[(i-3)*(_width-1) + j-3];
						pixelbuffer[3*i*_width + 3*j + 1] = scores[(i-3)*(_width-1) + j-3];
						pixelbuffer[3*i*_width + 3*j + 2] = scores[(i-3)*(_width-1) + j-3];
					}else{
						pixelbuffer[3*i*_width + 3*j + 0] = 0x0;
						pixelbuffer[3*i*_width + 3*j + 1] = 0x0;
						pixelbuffer[3*i*_width + 3*j + 2] = 0x0;
						
					}
				}
			}
		}		
	}



	for(i = 0; i < _height; i++){
		for(j = 0; j < _width; j+=1){

			uint8_t thresh = 0;
			if(scores[(i)*(_width) + j] > thresh ){
						pixelbuffer[3*i*_width + 3*j + 0] = scores[(i)*(_width) + j];
						pixelbuffer[3*i*_width + 3*j + 1] = scores[(i)*(_width) + j];
						pixelbuffer[3*i*_width + 3*j + 2] = scores[(i)*(_width) + j];
			}else{
						pixelbuffer[3*i*_width + 3*j + 0] = 0x0;
						pixelbuffer[3*i*_width + 3*j + 1] = 0x0;
						pixelbuffer[3*i*_width + 3*j + 2] = 0x0;
						
			}
		}
	}	


	fwrite(pixelbuffer,1,_pixelbytesize,fp);
	
    fclose(fp);
    free(pbitmap);
    free(pixelbuffer);

}

*/
