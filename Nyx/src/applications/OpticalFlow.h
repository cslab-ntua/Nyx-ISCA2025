#ifndef _OPTICALFLOW_H_
#define _OPTICALFLOW_H_

#include <stdint.h>
#include <stdio.h>
#include "../fpga/fpga_utils.h"
#include "ImageLib/imageLib.h"

#define OPTICALFLOW_INPUT 	0x0000000280010000
#define OPTICALFLOW_OUTPUT 	0x0000000C40010000

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