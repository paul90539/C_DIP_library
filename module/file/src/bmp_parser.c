#include "bmp_parser.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color_convert.h"

static uint8_t* bmp_load_ch1(FILE* fp, const uint32_t row, const uint32_t col) {

	uint8_t* data;
	uint8_t* data_ptr = NULL;
	uint32_t r;
	uint32_t ext_offset = (col & 3) ? 4 - (col & 3) : 0;

	data = malloc(row * col);
	if(data == NULL) return NULL;

	// I don't care about singal channel's color palette
	fseek(fp, 1078, SEEK_SET); 

	data_ptr = data;
	for(r = 0; r < row; r++, data_ptr += col) {
		fread(data_ptr, col, 1, fp);
		fseek(fp, ext_offset, SEEK_CUR);
	}

	return data;
}

static uint8_t* bmp_load_ch3(FILE* fp, const uint32_t row, const uint32_t col) {

	uint8_t* data;
	uint8_t* data_ptr = NULL;
	uint32_t r, c;
	uint32_t ext_offset = (col & 3);

	data = malloc(row * col * 3);
	if(data == NULL) return NULL;

	// 3-channels without color palette
	fseek(fp, 54, SEEK_SET);

	data_ptr = data;
	for(r = 0; r < row; r++, data_ptr += 3 * col) {
		fread(data_ptr, col * 3, 1, fp);
		fseek(fp, ext_offset, SEEK_CUR);
	}

	return data;
}


uint8_t* bmp_load_gray(const char* img_path, uint32_t* row, uint32_t* col, uint8_t* channels) {

	uint16_t bfType;
	BITMAP_FILE_HEADER file_header;
	BITMAP_INFO_HEADER info_header;

	FILE* fp = fopen(img_path, "rb");
	uint8_t* data = NULL;

	if(fp == NULL) return NULL;

	fread(&bfType, sizeof(uint16_t), 1, fp); // alway is "BM", same to 0x4D42
	if(bfType != 0x4D42) return NULL;

	fread(&file_header, sizeof(BITMAP_FILE_HEADER), 1, fp);
	fread(&info_header, sizeof(BITMAP_INFO_HEADER), 1, fp);

	*row = info_header.biHeight;
	*col = info_header.biWidth;
	*channels = info_header.biBitCount / 8;

	if(*channels == 1) {
		data = bmp_load_ch1(fp, *row, *col);
	}
	else if(*channels == 3)  {
		data = bmp_load_ch3(fp, *row, *col);
		rgb_to_gray(data, *row, *col);
	}
	else {
		return NULL;
	}

	fclose(fp);

	return data;
}

// only gray image
void bmp_save_gray(const char* img_path, uint8_t* data, const uint32_t row, const uint32_t col) {

	uint16_t bfType;
	BITMAP_FILE_HEADER file_header;
	BITMAP_INFO_HEADER info_header;

	uint32_t r, idx;
	uint32_t ext_offset = (col & 3) ? 4 - (col & 3) : 0;
	uint32_t ext_col = col + ext_offset;
	uint8_t color_palette[1024];

	FILE* fp = fopen(img_path, "wb");
	uint8_t* data_buf = NULL;
	uint8_t* data_ptr = NULL;

	data_buf = malloc(ext_col);

	if(fp == NULL) return;
	if(data_buf == NULL) return;
	memset(data_buf, 0, ext_col);

	// file header setting
	memset(&file_header, 0, sizeof(BITMAP_FILE_HEADER));
	bfType = 0x4D42;
	file_header.bfOffBits = sizeof(uint16_t) + sizeof(BITMAP_INFO_HEADER) + sizeof(BITMAP_FILE_HEADER) + 256 * 4;
	file_header.bfSize = file_header.bfOffBits + row * ext_col;
	
	// info header setting
	memset(&info_header, 0, sizeof(BITMAP_INFO_HEADER));
	info_header.biSize = sizeof(BITMAP_INFO_HEADER);
	info_header.biWidth = col;
	info_header.biHeight = row;
	info_header.biPlanes = 1;
	info_header.biBitCount = 8;
	info_header.biSizeImage = row * ext_col;

	// write header
	fwrite(&bfType, sizeof(uint16_t), 1, fp);
	fwrite(&file_header, sizeof(BITMAP_FILE_HEADER), 1, fp);
	fwrite(&info_header, sizeof(BITMAP_INFO_HEADER), 1, fp);

	// color palette setting
	for(r = 0, idx = 0; r < 256; r++, idx+=4) {
		color_palette[idx + 0] = r;
		color_palette[idx + 1] = r;
		color_palette[idx + 2] = r;
		color_palette[idx + 3] = 0;
	}

	// write color palette
	fwrite(&color_palette, 1024, 1, fp);

	// write data
	data_ptr = data;
	for(r = 0; r < row; r++, data_ptr += col) {
		memcpy(data_buf, data_ptr, col);
		fwrite(data_buf, ext_col, 1, fp);
	}

	free(data_buf);
	fclose(fp);
}