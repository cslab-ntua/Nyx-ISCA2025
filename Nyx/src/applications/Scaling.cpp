#include "Scaling.h"
#include "opencv2/highgui.hpp"

using namespace std;


void Scaling_input(){

        cv::Mat in_img;
        //Load input image
        //printf("Reading image...\n");
        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
        const string title = pwd + "/applications/input_image.jpeg";

	in_img = cv::imread(title, 1);
	if (in_img.data == NULL){
		fprintf(stderr, "Can not open image \n");
		return;
	}
	//Initialize function parameters
	uint32_t height = in_img.rows;
	uint32_t width = in_img.cols;
	uint32_t scale_factor = 2;
	uint32_t color = 0;

	uint32_t *img_input = (uint32_t*)malloc(sizeof(uint32_t) * height * width);

	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			img_input[i*width + j] = in_img.at<uint32_t>(i,j);

   	write_fpga(img_input, sizeof(uint32_t) * height * width, SCALING_INPUT);

	free(img_input);
}


void Scaling_output(){

	cv::Mat in_img;
	//Load input image
	//printf("Reading image...\n");
        const size_t size = 1024;
	char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
        const string title = pwd + "/applications/input_image.jpeg";

	in_img = cv::imread(title, 1);
	if (in_img.data == NULL){
		fprintf(stderr, "Can not open image \n");
		return;
	}
	//Initialize function parameters
	uint32_t height = in_img.rows;
	uint32_t width = in_img.cols;
	uint32_t scale_factor = 2;
	uint32_t color = 0;

	uint32_t *img_output = (uint32_t*)malloc(sizeof(uint32_t) * (scale_factor * height) * (scale_factor * width));


   	read_fpga(img_output, sizeof(uint32_t) * (scale_factor * height) * (scale_factor * width), SCALING_OUTPUT);

	cv::Mat out_hw(scale_factor*height, scale_factor*width, CV_8UC3, img_output);
	cv::imwrite(pwd + "/applications/output_image.jpeg", out_hw);

	free(img_output);

} 
