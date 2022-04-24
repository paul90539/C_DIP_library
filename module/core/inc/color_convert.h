#ifndef _COLOR_CONVERT_H_
#define _COLOR_CONVERT_H_

#include <stdint.h>

void rgb_to_gray(uint8_t* data, const uint32_t row, const uint32_t col);

#endif // _COLOR_CONVERT_H_