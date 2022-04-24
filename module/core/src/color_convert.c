#include "color_convert.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void rgb_to_gray(uint8_t* data, const uint32_t row, const uint32_t col) {

	uint32_t r, c, idx;
	uint8_t* rslt = NULL;
	uint8_t* data_ptr;
	uint8_t* data_slow_ptr;

	data_ptr = data;
	data_slow_ptr = data;

	for(r = 0, idx = 0; r < row; r++) {
		for(c = 0; c < col; c++, data_ptr+=3, data_slow_ptr++) {
			*data_slow_ptr = (uint8_t)(*(data_ptr + 0) * 0.299f + *(data_ptr + 1) * 0.587f + *(data_ptr + 2) * 0.114f);
		}
	}

	data = realloc(data, row * col);
}