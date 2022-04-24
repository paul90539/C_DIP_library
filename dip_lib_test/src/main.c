
#include "bmp_parser.h"

#include <stdint.h>

int main(void) {

	uint32_t row, col;
	uint8_t ch;

	uint8_t* data = bmp_load_gray("D:/workspace/dip_lib/dev_test/sample_3.bmp", &row, &col, &ch);
	bmp_save_gray("D:/workspace/dip_lib/dev_test/sample_out2.bmp", data, row, col);

	free(data);

	return 0;
}