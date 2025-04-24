#ifndef __COMPRESSION_H__
#define __COMPRESSION_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define DATAFLOW
//#define NONDATAFLOW

#define IMAGE_MAX_M 1920
#define IMAGE_MAX_N 1080
#define IMAGE_MAX IMAGE_MAX_M*IMAGE_MAX_N

typedef float DTYPE;
typedef float QTYPE;

#define DZERO 0.00f
#define QZERO 0

#define MAX_HEIGH 25
#define MAX_WIDTH 10


#ifdef NONDATAFLOW
void padImage(DTYPE *src, DTYPE *dst,  int srcm,  int srcn,  int dstm,  int dstn);
void extractBlockD(DTYPE *src, DTYPE *dst,  int srcm,  int srcn);
void dct(DTYPE *src, DTYPE *dst, int srcm, int srcn);
void idct(DTYPE *src, DTYPE *dst, int srcm, int srcn);
void quantize(DTYPE *src, QTYPE *dst, int srcm, int srcn);
void dequantize(QTYPE *src, DTYPE *dst, int srcm, int srcn);
void reconstructBlockD(DTYPE *src, DTYPE *dst,  int srcm,  int srcn);
#endif

#include <hls_stream.h>
#include "ap_int.h"
//STREAM FOR 256 BITS
typedef ap_uint<256> bit256;
typedef hls::stream<bit256> stream256;

#ifdef DATAFLOW

void padImage(DTYPE *src, stream256 &dst,  int srcm,  int srcn,  int dstm,  int dstn);
void extractBlockD(stream256 &src, stream256 &dst,  int srcm,  int srcn);
void dct(stream256 &src, stream256 &dst, int srcm, int srcn);
void idct(stream256 &src, stream256 &dst, int srcm, int srcn);
void quantize(stream256 &src, stream256 &dst, int srcm, int srcn);
void dequantize(stream256 &src, stream256 &dst, int srcm, int srcn);
void reconstructBlockD(stream256 &src, DTYPE *dst,  int srcm,  int srcn);

#endif

#endif
