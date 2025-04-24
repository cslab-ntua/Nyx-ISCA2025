/*===============================================================*/
/*                                                               */
/*                        typedefs.h                             */
/*                                                               */
/*        Defines types and constants for host function          */
/*                                                               */
/*===============================================================*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__
const int MAX_HEIGHT = 436;
const int MAX_WIDTH = 1024;

//#define SW
//#define APP
#define DATAFLOW
//#define NONDATAFLOW

// basic typedefs
#ifndef SW
	#include "hls_burst_maxi.h"
	#include "ap_fixed.h"
	//typedef ap_fixed<17,9> input_t; //TODO: Please check again the number before and after the decimal in input_t
	typedef ap_fixed<32,9> input_t;
	typedef ap_fixed<32,13> pixel_t;
	typedef ap_fixed<32,27> outer_pixel_t;
	typedef ap_fixed<64,56> calc_pixel_t;
	typedef ap_fixed<32,13> vel_pixel_t;
#else
	typedef float pixel_t;
	typedef float outer_pixel_t;
	typedef float vel_pixel_t;
#endif
typedef struct{
	pixel_t x;
	pixel_t y;
	pixel_t z;
#ifndef APP
	pixel_t w;//useless just for the port declaration
#endif
}gradient_t;

typedef struct{
#ifndef APP
    outer_pixel_t val[8]; //used to be 6
#else
    outer_pixel_t val[6];
#endif
}outer_t;

typedef struct{
#ifndef APP
    outer_pixel_t val[8]; //used to be 6
#else
    outer_pixel_t val[6];
#endif
}tensor_t;

typedef struct{
    vel_pixel_t x;
    vel_pixel_t y;
}velocity_t;

#ifndef SW
  #include "ap_int.h"
  // for data packing
  typedef ap_uint<64> frames_t;
#endif

//top level decleration
#ifdef APP
void optical_flow(frames_t frames[MAX_HEIGHT * MAX_WIDTH], velocity_t outputs[MAX_HEIGHT * MAX_WIDTH]);
#endif

#ifdef VITIS

#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef ap_uint<256> bit256;
typedef hls::stream<bit256> STREAM256;

#endif

#ifdef DATAFLOW

#include <hls_stream.h>
#include <ap_axi_sdata.h>

//STREAM FOR 256 BITS
typedef ap_uint<256> bit256;
typedef ap_axiu<256,1,2,2> AXI256;
typedef hls::stream<bit256> STREAM256;

void read_preprocess(frames_t* frames, STREAM256 &frame12_45, STREAM256 &frame3);
void gradient_xy_calc(STREAM256 &frame, STREAM256 &gradient_xy);
void gradient_z_calc(STREAM256 &frame12_45, STREAM256 &gradient_z);
void gradient_weight_y(STREAM256 &gradient_xy,STREAM256 &gradient_z, STREAM256 &filt_grad);
void gradient_weight_x(STREAM256 &y_filt, STREAM256 &filt_grad);
void outer_product(STREAM256 &gradient, STREAM256 &out_product);
void tensor_weight_y(STREAM256 &outer, STREAM256 &tensor_y);
void tensor_weight_x(STREAM256 &tensor_y, STREAM256 &tensor);
void flow_calc(STREAM256 &tensors, velocity_t* outputs);

#endif

#ifdef NONDATAFLOW

typedef struct{
	input_t val[4];
}input4_t;

typedef struct{
	pixel_t val[2];
}pixel2_t;

void read_preprocess(frames_t* frames, input4_t* frame12_45, input_t* frame3);
void gradient_xy_calc(input_t* frame3,	pixel2_t* gradient_xy);
void gradient_z_calc(input4_t* frame12_45, pixel_t* gradient_z);
void gradient_weight_y(pixel2_t* gradient_xy, pixel_t* gradient_z, gradient_t* filt_grad);
void gradient_weight_x(gradient_t* y_filt, gradient_t* filt_grad);
void outer_product(gradient_t* gradient, outer_t* out_product);
void tensor_weight_y(outer_t* outer, tensor_t* tensor_y);
void tensor_weight_x(tensor_t* tensor_y, tensor_t* tensor);
void flow_calc(tensor_t* tensors, velocity_t* outputs);

#endif
#endif

