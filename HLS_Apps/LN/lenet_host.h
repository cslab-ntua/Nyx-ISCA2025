#ifndef LENET_HOST_H
#define LENET_HOST_H

#define DATAFLOW
//#define NONDATAFLOW

#include <stdio.h>
#include <stdlib.h>
//#include <hls_video.h>
#include <fstream>
#include <string>
using namespace std;

#include "ap_fixed.h"

#define CUSTOM_EXP_WIDTH	16
#define CUSTOM_INT_WIDTH	5

typedef ap_fixed<CUSTOM_EXP_WIDTH, CUSTOM_INT_WIDTH> float24_t;


#define IMAGE_SIZE 			32
#define IMAGE_CHANNELS		1

#define CONV1_KERNEL_SIZE 	4
#define CONV1_CHANNELS 		1
#define CONV1_FILTERS 		8
#define CONV1_BIAS_SIZE 	8
#define CONV1_STRIDE 		1

#define A1_SIZE				29
#define A1_CHANNELS			8


#define P1_SIZE				14
#define P1_CHANNELS			8
#define P1_KERNEL_SIZE		2
#define P1_STRIDE			2


#define CONV2_KERNEL_SIZE 	2
#define CONV2_CHANNELS 		8
#define CONV2_FILTERS 		16
#define CONV2_BIAS_SIZE		16
#define CONV2_STRIDE 		1


#define A2_SIZE				13
#define A2_CHANNELS			16

#define P2_SIZE				6
#define P2_CHANNELS			16
#define P2_KERNEL_SIZE		2
#define P2_STRIDE			2

#define FLATTEN_SIZE		576

#define FC1_WEIGHTS_H		576
#define FC1_WEIGHTS_W		120
#define FC1_BIAS_SIZE		120
#define FC1_OUT_SIZE		120


#define FC2_WEIGHTS_H		120
#define FC2_WEIGHTS_W		84
#define FC2_BIAS_SIZE		84
#define FC2_OUT_SIZE		84


#define FC3_WEIGHTS_H		84
#define FC3_WEIGHTS_W		10
#define FC3_BIAS_SIZE		10

#define FC1_ACT_SIZE		120
#define FC2_ACT_SIZE		84
#define FC3_ACT_SIZE		10

#define CONV1_BUFFER_SIZE (IMAGE_SIZE*IMAGE_SIZE*CONV1_CHANNELS)
#define CONV2_BUFFER_SIZE (P1_SIZE * P1_SIZE * CONV2_CHANNELS)

#define POOL1_BUFFER_SIZE (A1_SIZE*A1_SIZE)
#define POOL2_BUFFER_SIZE (A2_SIZE*A2_SIZE)

#include <hls_stream.h>
#include "ap_int.h"

typedef ap_fixed<256, 77> bit256;
//typedef ap_uint<256> bit256;
typedef hls::stream<bit256> stream256;

#ifdef DATAFLOW

void conv1(float *image_in, stream256 &conv1_out);
void pool1(stream256 &conv1_out, stream256 &pool1_out);
void conv2(stream256 &pool1_out, stream256 &conv2_out);
void pool2(stream256 &conv2_out, stream256 &pool2_out);
void fc1(stream256 &pool2_out, stream256 &fc1_out);
void fc2(stream256 &fc1_out, stream256 &fc2_out);
void fc3(stream256 &fc2_out, float *fc3_out);

#endif

#ifdef NONDATAFLOW
void conv1(float *image_in, float *conv1_out);
void pool1(float *conv1_out, float *pool1_out);
void conv2(float *pool1_out, float *conv2_out);
void pool2(float *conv2_out, float *pool2_out);
void fc1(float *pool2_out, float *fc1_out);
void fc2(float *fc1_out, float *fc2_out);
void fc3(float *fc2_out, float *fc3_out);
#endif

#endif
