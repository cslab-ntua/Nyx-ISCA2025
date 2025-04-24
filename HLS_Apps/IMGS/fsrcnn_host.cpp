///OpenCV - Xilinx custom version
#include "opencv2/highgui.hpp"

///Software tools for testbench - They must be in this order
#include "common/xf_sw_utils.h"
#include "common/xf_axi.h"
///Import wrapper
#include "wrapper.hpp"
#include <chrono>
using namespace std::chrono;

int main(){

	cv::Mat in_img;
	//Load input image
	printf("Reading image...\n");

	const string title = "./input_image.jpeg";
	in_img = cv::imread(title, 1);
	if (in_img.data == NULL){
		fprintf(stderr, "Can not open image \n");
		return -1;
	}
	//Initialize function parameters
	uint32_t height = in_img.rows;
	uint32_t width = in_img.cols;
	uint32_t scale_factor = 2;
	uint32_t color = 0;

	#ifdef APPLICATION

	cv::Mat out_hw;

	//Create the hardware streams
	out_hw.create(scale_factor*height, scale_factor*width, CV_8UC3);
	//Generate the streams from the input image
	stream_t src_hw, sink_hw;
	cvMat2AXIvideoxf<NPC1>(in_img, src_hw);
	//Execute accelerator
	FSRCNN(src_hw, sink_hw, height, width, scale_factor);
	//Retrieve the output and put it into a cv::Mat
	AXIvideo2cvMatxf<NPC1>(sink_hw, out_hw);
	#endif


	#ifdef DATAFLOW
	uint32_t *img_input = new uint32_t[height*width];
	uint32_t *img_output = new uint32_t[(scale_factor*height)*(scale_factor*width)];

    stream256 corr1_out("corr1_out");
    stream256 corr2_out("corr2_out");
    stream256 corr3_out("corr3_out");
    stream256 corr4_out("corr4_out");
    stream256 corr5_out("corr5_out");
    stream256 corr6_out("corr6_out");
    stream256 corr7_out("corr7_out");
    stream256 corr8_out("corr8_out");


	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			img_input[i*width + j] = in_img.at<uint32_t>(i,j);

	conrr_layer1(img_input, corr1_out, height, width, color);
	conrr_layer2(corr1_out, corr2_out, height, width, color);
	conrr_layer3(corr2_out, corr3_out, height, width, color);
	conrr_layer4(corr3_out, corr4_out, height, width, color);
	conrr_layer5(corr4_out, corr5_out, height, width, color);
	conrr_layer6(corr5_out, corr6_out, height, width, color);
	conrr_layer7(corr6_out, corr7_out, height, width, color);
	decorr_layer8(corr7_out, corr8_out, height, width, scale_factor, color);
	uncon_layer9(corr8_out, img_output, height, width, color);

	cv::Mat out_hw(scale_factor*height, scale_factor*width, CV_8UC3, img_output);



	//for(int i=0; i<MAX_OUT_HEIGHT; i++)
	//	for(int j=0; j<MAX_OUT_WIDTH; j++)
	//		out_hw.at<uint32_t>(i,j) = img_output[i*MAX_OUT_WIDTH + j];


	delete []img_input;
	delete []img_output;
	#endif

	#ifdef NONDATAFLOW
	uint32_t *img_input = new uint32_t[height*width];
	uint32_t *img_output = new uint32_t[(scale_factor*height)*(scale_factor*width)];

	for(int i=0; i<height; i++)
		for(int j=0; j<width; j++)
			img_input[i*width + j] = in_img.at<uint32_t>(i,j);


	my_type *corr1_out = new my_type[229376];
	conrr_layer1(img_input, corr1_out, height, width, color);
	my_type *corr2_out = new my_type[229376];
	conrr_layer2(corr1_out, corr2_out, height, width, color);
	delete []corr1_out;
	my_type *corr3_out = new my_type[229376];
	conrr_layer3(corr2_out, corr3_out, height, width, color);
	delete []corr2_out;
	my_type *corr4_out = new my_type[229376];
	conrr_layer4(corr3_out, corr4_out, height, width, color);
	delete []corr3_out;
	my_type *corr5_out = new my_type[229376];
	conrr_layer5(corr4_out, corr5_out, height, width, color);
	delete []corr4_out;
	my_type *corr6_out = new my_type[229376];
	conrr_layer6(corr5_out, corr6_out, height, width, color);
	delete []corr5_out;
	my_type *corr7_out = new my_type[229376];
	conrr_layer7(corr6_out, corr7_out, height, width, color);
	delete []corr6_out;
	my_type *corr8_out = new my_type[229376];
	decorr_layer8(corr7_out, corr8_out, height, width, scale_factor, color);
	delete []corr7_out;
	uncon_layer9(corr8_out, img_output, height, width, color);
	delete []corr8_out;

	cv::Mat out_hw(scale_factor*height, scale_factor*width, CV_8UC3, img_output);

	#endif

	cv::imwrite("./output_image.jpeg", out_hw);

	return 0;

}
