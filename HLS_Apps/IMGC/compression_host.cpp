/*===============================================================*/
/*                                                               */
/*                    digit_recognition.cpp                      */
/*                                                               */
/*   Main host function for the Digit Recognition application.   */
/*                                                               */
/*===============================================================*/

// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <time.h>
#include <sys/time.h>
#include <math.h>

// other headers
#include "typedefs.h"

// Image compression structures -- Need to be 64-bit aligned as we use 64-bit data bus
int W = 16;
int H = 32;

void init_img_cmp_data(float *img_cmp_src, float *img_cmp_dst, float *img_cmp_golden, int max_height, int max_width) {

	srand(0);
	float rand_0;

	for(int h = 0; h < max_height; h++) {
		rand_0 = rand() * 254.0 / RAND_MAX + 1;
		for(int w = 0; w < max_width; w++) {
			img_cmp_src[h * max_width + w] = 0.5;
			img_cmp_golden[h * max_width + w] = 0.5;
			img_cmp_dst[h * max_width + w] = 0.0f;
		}
	}
}

float img_cmp_check_result(float *golden, float *test, int max_width, int max_height){
	int w, h;
	float cum_error = 0.0;
	float difference = 0.0;
	for (h = 0; h < max_height; h++){
		for (w = 0; w < max_width; w++){
			difference = golden[h * W + w] - test[h * W + w];
			if (golden[h * W + w] == 0){
				printf("Error! 0 input!\r\n");
			}
			cum_error += sqrt(difference * difference) / golden[h * W + w];
		}
	}

    return cum_error / (W * H);
}


int main(int argc, char ** argv){

	printf("Image Compression Application\n");

    float* img_cmp_src = new float[IMAGE_MAX];
    float* img_cmp_dst = new float[IMAGE_MAX];
    float* img_cmp_golden = new float[IMAGE_MAX];

    init_img_cmp_data(img_cmp_src, img_cmp_dst, img_cmp_golden, 25, 10);

  	#ifdef DATAFLOW

    stream256 pagImg_dst("pagImg_dst");
    padImage(img_cmp_src, pagImg_dst,  25,  10,  W,  H);
    stream256 BlockD_dst("BlockD_dst");
    extractBlockD(pagImg_dst, BlockD_dst,  W,  H);
    stream256 dct_dest("dct_dest");
    dct(BlockD_dst, dct_dest, W,  H);
    stream256 quantize_dest("quantize_dest");
    quantize(dct_dest, quantize_dest, W,  H);
    stream256 dequantize_dest("dequantize_dest");
    dequantize(quantize_dest, dequantize_dest, W,  H);
    stream256 idct_dest("idct_dest");
    idct(dequantize_dest, idct_dest, W,  H);
    reconstructBlockD(idct_dest, img_cmp_dst, W, H);


  	#endif

	#ifdef NONDATAFLOW

    float* pagImg_dst = new float[IMAGE_MAX];
    padImage(img_cmp_src, pagImg_dst,  25,  10,  W,  H);
    float* BlockD_dst = new float[IMAGE_MAX];
    extractBlockD(pagImg_dst, BlockD_dst,  W,  H);
    delete []pagImg_dst;
    float* dct_dst = new float[IMAGE_MAX];
    dct(BlockD_dst, dct_dst, W,  H);
    delete []BlockD_dst;
    float* quantize_dst = new float[IMAGE_MAX];
    quantize(dct_dst, quantize_dst, W,  H);
    delete []dct_dst;
    float* dequantize_dst = new float[IMAGE_MAX];
    dequantize(quantize_dst, dequantize_dst, W,  H);
    delete []quantize_dst;
    float* idct_dst = new float[IMAGE_MAX];
    idct(dequantize_dst, idct_dst, W,  H);
    delete []dequantize_dst;
    reconstructBlockD(idct_dst, img_cmp_dst, W, H);
    delete []idct_dst;
    #endif



    // check results
    printf("Checking results:\n");
	printf("Img compression has a cumulative error of %f\n\r", img_cmp_check_result(img_cmp_golden, img_cmp_dst, 25, 10));
    //img_cmp_check_result(img_cmp_golden, img_cmp_dst, 25, 10);
	delete []img_cmp_src;
	delete []img_cmp_dst;
	delete []img_cmp_golden;

    return EXIT_SUCCESS;
}
