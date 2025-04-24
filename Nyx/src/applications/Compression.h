#ifndef _COMPRESSION_H_
#define _COMPRESSION_H_

#include <stdint.h>
#include <stdio.h>
#include "../fpga/fpga_utils.h"

#define COMPRESSION_INPUT   0x0000000180010000
#define COMPRESSION_OUTPUT 0x0000000740010000

#define IMAGE_MAX_M 1920
#define IMAGE_MAX_N 1080
#define IMAGE_MAX IMAGE_MAX_M*IMAGE_MAX_N

typedef float DTYPE;
typedef float QTYPE;

#define DZERO 0.00f
#define QZERO 0

const int W = 16;
const int H = 32;

void Compression_input();
void Compression_output();


#endif
