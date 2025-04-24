/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*                     Typedefs for the host                     */
/*                                                               */
/*===============================================================*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

// resolution 256x256
const int MAX_X = 256;
const int MAX_Y = 256;

// number of values in frame buffer: 32 bits
const int NUM_FB = MAX_X * MAX_Y / 4;
// dataset information
const int NUM_3D_TRI = 3192;

//#define SW
//#define APP
#define DATAFLOW
//#define NONDATAFLOW

#ifndef SW
  // hls header
  #include "ap_int.h"
  // specialized datatypes
  typedef ap_uint<1> bit1;
  typedef ap_uint<2> bit2;
  typedef ap_uint<8> bit8;
  typedef ap_uint<16> bit16;
  typedef ap_uint<32> bit32;
  typedef ap_uint<128> bit128;

#else
  typedef unsigned char bit8;
  typedef unsigned int bit32;
#endif

// struct: 3D triangle
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

// struct: candidate pixels
typedef struct
{
  bit8   x;
  bit8   y;
  bit8   z;
  bit8   color;
} CandidatePixel;

// struct: colored pixel
typedef struct
{
  bit8   x;
  bit8   y;
  bit8   color;
} Pixel;

// dataflow switch
#define ENABLE_DATAFLOW

#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "hls_burst_maxi.h"
//STREAM FOR 256 BITS
typedef ap_uint<256> bit256;
typedef hls::stream<bit256> STREAM256;

#ifdef APP
void rendering(bit32 input[3*NUM_3D_TRI], bit32 output[NUM_FB]);
#endif

#ifdef DATAFLOW
void projection_rasterization(bit32 *input, STREAM256 &fragment2, STREAM256 &size_fragment);
void zculling(STREAM256 &fragment2, STREAM256 &size_fragment, STREAM256 &pixels, STREAM256 &size_pixels);
void coloring_outputFB(STREAM256 &pixels, STREAM256 &size_pixels, bit32 *output);
#endif

#ifdef NONDATAFLOW

void projection_rasterization(bit32 *input, CandidatePixel *fragment2, bit16 *size_fragment);
void zculling(CandidatePixel *fragment2, bit16 *size_fragment, Pixel *pixels, bit16 *size_pixels);
void coloring_outputFB(Pixel *pixels, bit16 *size_pixels, bit32 *output);

#endif


#endif
