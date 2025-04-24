/*===============================================================*/
/*                                                               */
/*                       3d_rendering.cpp                        */
/*                                                               */
/*      Main host function for the 3D Rendering application.     */
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
#include "input_data.h"
#include "check_result.h"

#ifdef SW
  #include "rendering_sw.h"
#endif


int main(int argc, char ** argv){

	printf("3D Rendering Application\n");

  	#ifdef SW

    bit8 output[MAX_X][MAX_Y];
    rendering_sw(triangle_3ds, output);


	#else

	#ifdef APP

    // create space for input and output
    bit32* input  = new bit32[3 * NUM_3D_TRI];
    bit32* output = new bit32[NUM_FB];

    // pack input data for better performance
    for ( int i = 0; i < NUM_3D_TRI; i++){
    	input[3*i](7,0)     = triangle_3ds[i].x0;
    	input[3*i](15,8)    = triangle_3ds[i].y0;
    	input[3*i](23,16)   = triangle_3ds[i].z0;
    	input[3*i](31,24)   = triangle_3ds[i].x1;
    	input[3*i+1](7,0)   = triangle_3ds[i].y1;
    	input[3*i+1](15,8)  = triangle_3ds[i].z1;
    	input[3*i+1](23,16) = triangle_3ds[i].x2;
    	input[3*i+1](31,24) = triangle_3ds[i].y2;
    	input[3*i+2](7,0)   = triangle_3ds[i].z2;
    	input[3*i+2](31,8)  = 0;
    }

    rendering(input, output);

	#endif

    // create space for input and output
    bit32* input  = new bit32[3*NUM_3D_TRI];
    bit32* output = new bit32[NUM_FB];

    // pack input data for better performance
    for ( int i = 0; i < NUM_3D_TRI; i++){
    	input[3*i](7,0)     = triangle_3ds[i].x0;
    	input[3*i](15,8)    = triangle_3ds[i].y0;
    	input[3*i](23,16)   = triangle_3ds[i].z0;
    	input[3*i](31,24)   = triangle_3ds[i].x1;
    	input[3*i+1](7,0)   = triangle_3ds[i].y1;
    	input[3*i+1](15,8)  = triangle_3ds[i].z1;
    	input[3*i+1](23,16) = triangle_3ds[i].x2;
    	input[3*i+1](31,24) = triangle_3ds[i].y2;
    	input[3*i+2](7,0)   = triangle_3ds[i].z2;
    	input[3*i+2](31,8)  = 0;
    }

	#ifdef DATAFLOW

    STREAM256 fragment2("fragment2");
    STREAM256 size_fragment("size_fragment");
    projection_rasterization(input, fragment2, size_fragment);
    STREAM256 pixels("pixels");
    STREAM256 size_pixels("size_pixels");
    zculling(fragment2, size_fragment, pixels, size_pixels);
    coloring_outputFB(pixels, size_pixels, output);
  	#endif

	#ifdef NONDATAFLOW
    CandidatePixel* fragment2 = new CandidatePixel[NUM_3D_TRI*500];
    bit16* size_fragment = new bit16[NUM_3D_TRI];
	projection_rasterization(input, fragment2, size_fragment);
	Pixel* pixels = new Pixel[NUM_3D_TRI*500];
	bit16* size_pixels = new bit16[NUM_3D_TRI];
	zculling(fragment2, size_fragment, pixels, size_pixels);
	delete[] fragment2;
	delete[] size_fragment;
	coloring_outputFB(pixels, size_pixels, output);
	delete[] pixels;
	delete[] size_pixels;
	#endif

	#endif

    //check results
    printf("Checking results:\n");
    check_results(output);

	#ifndef SW
	delete[] input;
	delete[] output;
	#endif

    return EXIT_SUCCESS;

}
