#include "Compression.h"

#include <iostream>
#include <cmath>

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

void Compression_input(){

	float *img_cmp_src = (float*)malloc(sizeof(float) * IMAGE_MAX);
	float *img_cmp_dst = (float*)malloc(sizeof(float) * IMAGE_MAX);
	float *img_cmp_golden = (float*)malloc(sizeof(float) * IMAGE_MAX);

    init_img_cmp_data(img_cmp_src, img_cmp_dst, img_cmp_golden, 25, 10);

   	write_fpga(img_cmp_src, sizeof(float) * IMAGE_MAX, COMPRESSION_INPUT);

	free(img_cmp_src);
	free(img_cmp_dst);
	free(img_cmp_golden);

}

void Compression_output(){

	float *img_cmp_src = (float*)malloc(sizeof(float) * IMAGE_MAX);
	float *img_cmp_dst = (float*)malloc(sizeof(float) * IMAGE_MAX);
	float *img_cmp_golden = (float*)malloc(sizeof(float) * IMAGE_MAX);

    init_img_cmp_data(img_cmp_src, img_cmp_dst, img_cmp_golden, 25, 10);

   	//read_fpga(img_cmp_dst, sizeof(float)*IMAGE_MAX, COMPRESSION_OUTPUT);

    printf("Checking results:\n");
	printf("Img compression has a cumulative error of %f\n\r", img_cmp_check_result(img_cmp_golden, img_cmp_dst, 25, 10));

	free(img_cmp_src);
	free(img_cmp_dst);
	free(img_cmp_golden);

}