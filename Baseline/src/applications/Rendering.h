#ifndef _RENDERING_H_
#define _RENDERING_H_

#include <stdint.h>
#include <stdio.h>
#include "../fpga/fpga_utils.h"

// resolution 256x256
const int MAX_X = 256;
const int MAX_Y = 256;

// number of values in frame buffer: 32 bits
const int NUM_FB = MAX_X * MAX_Y / 4;
// dataset information
const int NUM_3D_TRI = 3192;

#define RENDERING_INPUT 	         0x0000000140010000
#define RENDERING_FRAGMENT         0x0000000040010000
#define RENDERING_SIZE_FRAGMENT    0x0000000400010000
#define RENDERING_PIXELS           0x0000000200010000
#define RENDERING_SIZE_PIXELS      0x0000000800010000
#define RENDERING_OUTPUT 	         0x0000000540010000

typedef unsigned char bit8;
typedef unsigned int bit32;

typedef struct
{
  bit8   x0;
  bit8   y0;
  bit8   z0;
  bit8   x1;
  bit8   y1;
  bit8   z1;
  bit8   x2;
  bit8   y2;
  bit8   z2;
} Triangle_3D;

typedef struct
{
  bit8   x0;
  bit8   y0;
  bit8   z0;
  bit8   x1;
  bit8   y1;
  bit8   z1;
  bit8   x2;
  bit8   y2;
  bit8   z2;
  bit8   k0;
  bit8	 k1;
  bit8 	 k2;
} Triangle_4D;

// struct: 2D triangle
typedef struct
{
  bit8   x0;
  bit8   y0;
  bit8   x1;
  bit8   y1;
  bit8   x2;
  bit8   y2;
  bit8   z;
} Triangle_2D;

void Rendering_input();
void Rendering_output();

#endif