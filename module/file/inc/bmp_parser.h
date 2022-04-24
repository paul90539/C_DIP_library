#ifndef _BMP_PARSER_H_
#define _BMP_PARSER_H_

#include <stdint.h>

typedef struct _BITMAP_FILE_HEADER
{
	//bType is check code "BM". with memory alignment to 4 byte, can't set this paramemter in structure.
	//uint16_t bfType; 
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} BITMAP_FILE_HEADER;

typedef struct _BITMAP_INFO_HEADER
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	uint32_t biXPelsPerMeter;
	uint32_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} BITMAP_INFO_HEADER;

// only can load 1-channel grey image or 3-channels color image
uint8_t* bmp_load_gray(const char* img_path, uint32_t* row, uint32_t* col, uint8_t* channels);

void bmp_save_gray(const char* img_path, uint8_t* data, const uint32_t row, const uint32_t col);


#endif //_BMP_PARSER_H_