/*===============================================================*/
/*                                                               */
/*                    optical_flow_host.cpp                      */
/*                                                               */
/*      Main host function for the Optical Flow application.     */
/*                                                               */
/*===============================================================*/

// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <time.h>
#include <sys/time.h>

// other headers
#include "typedefs.h"
#include "check_result.h"
#include "hls_burst_maxi.h"


#ifdef SW
#include "optical_flow_sw.h"
#endif

int main(int argc, char ** argv){

	printf("Optical Flow Application\n");
	//parse command line arguments

	std::string outFile("./datasets/current/output.flo");

	//create actual file names according to the datapath
	std::string frame_files[5];
	std::string reference_file;
	frame_files[0] = "./datasets/current/frame1.ppm"; //sintel_alley or current
	frame_files[1] = "./datasets/current/frame2.ppm";
	frame_files[2] = "./datasets/current/frame3.ppm";
	frame_files[3] = "./datasets/current/frame4.ppm";
	frame_files[4] = "./datasets/current/frame5.ppm";
	reference_file = "./datasets/current/ref.flo";

	// read in images and convert to grayscale
	printf("Reading input files ... \n");

	CByteImage imgs[5];
	for (int i = 0; i < 5; i++){
		CByteImage tmpImg;
		ReadImage(tmpImg, frame_files[i].c_str());
		imgs[i] = ConvertToGray(tmpImg);
	}

	//read in reference flow file
	printf("Reading reference output flow... \n");

	CFloatImage refFlow;
	ReadFlowFile(refFlow, reference_file.c_str());

#ifndef SW
    // input
    frames_t  frames[MAX_HEIGHT * MAX_WIDTH];
    // output
    velocity_t outputs [MAX_HEIGHT * MAX_WIDTH];

    // pack the values
    for (int i = 0; i < MAX_HEIGHT; i++){
    	for (int j = 0; j < MAX_WIDTH; j++){
    		frames[i*MAX_WIDTH+j](7 ,  0) = imgs[0].Pixel(j, i, 0);
			frames[i*MAX_WIDTH+j](15,  8) = imgs[1].Pixel(j, i, 0);
			frames[i*MAX_WIDTH+j](23, 16) = imgs[2].Pixel(j, i, 0);
			frames[i*MAX_WIDTH+j](31, 24) = imgs[3].Pixel(j, i, 0);
			frames[i*MAX_WIDTH+j](39, 32) = imgs[4].Pixel(j, i, 0);
			frames[i*MAX_WIDTH+j](63, 40) = 0;
    	}
    }

#endif

#ifdef APP
    optical_flow(frames, outputs);
#endif

#ifdef DATAFLOW

    STREAM256 frame3("frame3");
    STREAM256 frame12_45("frame12_45");
    read_preprocess(frames, frame12_45, frame3);

    STREAM256 gradient_xy("gradient_xy");
    STREAM256 gradient_z("gradient_z");
    gradient_xy_calc(frame3, gradient_xy);
    gradient_z_calc(frame12_45, gradient_z);

    STREAM256 y_filtered("y_filtered");
    STREAM256 filtered_gradient("filtered_gradient");
    gradient_weight_y(gradient_xy, gradient_z, y_filtered);
    gradient_weight_x(y_filtered, filtered_gradient);

    STREAM256 out_product("out_product");
    outer_product(filtered_gradient, out_product);

    STREAM256 tensor_y("tensor_y");
    tensor_weight_y(out_product, tensor_y);

    STREAM256 tensor_x("tensor_x");
    tensor_weight_x(tensor_y, tensor_x);

    flow_calc(tensor_x, outputs);


#endif


#ifdef SW

  // sw version host code
    static pixel_t frames[5][MAX_HEIGHT][MAX_WIDTH];
    static velocity_t outputs[MAX_HEIGHT][MAX_WIDTH];
    // use native C datatype arrays
    for (int f = 0; f < 5; f ++ )
      for (int i = 0; i < MAX_HEIGHT; i ++ )
        for (int j = 0; j < MAX_WIDTH; j ++ )
          frames[f][i][j] = imgs[f].Pixel(j, i, 0) / 255.0f;

    // run
    optical_flow_sw(frames[0], frames[1], frames[2], frames[3], frames[4], outputs);
#endif

#ifdef NONDATAFLOW

    input4_t *frame12_45 = (input4_t*) malloc(sizeof(input4_t)*MAX_HEIGHT * MAX_WIDTH);
    input_t *frame3 = (input_t*) malloc(sizeof(input_t)*MAX_HEIGHT * MAX_WIDTH);
    read_preprocess(frames, frame12_45, frame3);

    pixel2_t *gradient_xy = (pixel2_t*) malloc(sizeof(pixel2_t)*MAX_HEIGHT * MAX_WIDTH);
    gradient_xy_calc(frame3, gradient_xy);
    free(frame3);

    pixel_t *gradient_z = (pixel_t*) malloc(sizeof(pixel_t)*MAX_HEIGHT * MAX_WIDTH);
    gradient_z_calc(frame12_45, gradient_z);
    free(frame12_45);

    gradient_t *y_filt = (gradient_t*) malloc(sizeof(gradient_t)*MAX_HEIGHT * MAX_WIDTH);
    gradient_weight_y(gradient_xy, gradient_z, y_filt);
    free(gradient_xy);
    free(gradient_z);

    gradient_t *filt_grad = (gradient_t*) malloc(sizeof(gradient_t)*MAX_HEIGHT * MAX_WIDTH);
    gradient_weight_x(y_filt, filt_grad);
    free(y_filt);

    outer_t *out_product = (outer_t*) malloc(sizeof(outer_t)*MAX_HEIGHT * MAX_WIDTH);
    outer_product(filt_grad, out_product);
    free(filt_grad);

    tensor_t *tensor_y = (tensor_t*) malloc(sizeof(tensor_t)*MAX_HEIGHT * MAX_WIDTH);
    tensor_weight_y(out_product, tensor_y);
    free(out_product);

    tensor_t *tensor = (tensor_t*) malloc(sizeof(tensor_t)*MAX_HEIGHT * MAX_WIDTH);
    tensor_weight_x(tensor_y, tensor);
    free(tensor_y);

    flow_calc(tensor, outputs);
    free(tensor);

#endif

	// check results
	printf("Checking results:\n");
	check_results(outputs, refFlow, outFile);

  	//cleanup
    return EXIT_SUCCESS;

}
