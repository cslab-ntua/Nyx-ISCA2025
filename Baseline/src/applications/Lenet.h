#ifndef _LENET_H_
#define _LENET_H_

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include "../fpga/fpga_utils.h"


using namespace std;

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


#define LENET_INPUT   	0x0000000340010000
#define LENET_CONV1 	0x0000000640010000
#define LENET_POOL1 	0x0000000840010000
#define LENET_CONV2 	0x0000000940010000
#define LENET_POOL2 	0x0000000A00010000
#define LENET_FC1 		0x0000000B00010000
#define LENET_FC2 		0x0000000C00010000
#define LENET_OUTPUT 	0x0000000D00010000

void Lenet_input();
void Lenet_output();

#endif