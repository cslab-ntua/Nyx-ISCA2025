#ifndef _OPTICALFLOW_H_
#define _OPTICALFLOW_H_

#include <stdint.h>
#include <stdio.h>
#include "../fpga/fpga_utils.h"
#include "ImageLib/imageLib.h"

#define OPTICALFLOW_INPUT       0x0000000280010000
#define OPTICALFLOW_FRAME12_45  0x0000000380010000
#define OPTICALFLOW_FRAME_3     0x0000000480010000
#define OPTICALFLOW_GRADIENT_XY 0x0000000580010000
#define OPTICALFLOW_GRADIENT_Z  0x0000000680010000
#define OPTICALFLOW_FILT_GRAD_Y 0x0000000780010000
#define OPTICALFLOW_FILT_GRAD_X 0x0000000880010000
#define OPTICALFLOW_OUT_PRODUCT 0x0000000980010000
#define OPTICALFLOW_TENSOR_Y    0x0000000A40010000
#define OPTICALFLOW_TENSOR_X    0x0000000B40010000
#define OPTICALFLOW_OUTPUT      0x0000000C40010000

const int MAX_HEIGHT = 436;
const int MAX_WIDTH = 1024;

typedef float pixel_t;
typedef float outer_pixel_t;
typedef float vel_pixel_t;

typedef struct{
    vel_pixel_t x;
    vel_pixel_t y;
}velocity_t;

void OpticalFlow_input();
void OpticalFlow_output();


#endif