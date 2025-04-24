#include "OpticalFlow.h"

#include <iostream>
#include <cmath>

void OpticalFlow_input(){

        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
        std::string frame_files[5];

        cout << pwd << endl;
        frame_files[0] = pwd + "/applications/optflow_dataset/current/frame1.ppm"; //sintel_alley or current
        frame_files[1] = pwd + "/applications/optflow_dataset/current/frame2.ppm";
        frame_files[2] = pwd + "/applications/optflow_dataset/current/frame3.ppm";
        frame_files[3] = pwd + "/applications/optflow_dataset/current/frame4.ppm";
        frame_files[4] = pwd + "/applications/optflow_dataset/current/frame5.ppm";

	CByteImage imgs[5];
	for (int i = 0; i < 5; i++){
		CByteImage tmpImg;
		ReadImage(tmpImg, frame_files[i].c_str());
		imgs[i] = ConvertToGray(tmpImg);
	}

	pixel_t *frames = (pixel_t*)malloc(sizeof(pixel_t) * 5 * MAX_HEIGHT * MAX_WIDTH);


	for (int f = 0; f < 5; f ++ )
    	for (int i = 0; i < MAX_HEIGHT; i ++ )
        	for (int j = 0; j < MAX_WIDTH; j ++ )
          		frames[f*MAX_HEIGHT*MAX_WIDTH + i*MAX_WIDTH + j] = imgs[f].Pixel(j, i, 0) / 255.0f;


	write_fpga(frames, sizeof(pixel_t) * 5 * MAX_HEIGHT * MAX_WIDTH, OPTICALFLOW_INPUT);

	free(frames);

}

void check_results(velocity_t *output, CFloatImage refFlow, std::string outFile){

	CFloatImage outFlow(MAX_WIDTH, MAX_HEIGHT, 2);

	for (int i = 0; i < MAX_HEIGHT; i++){
		for (int j = 0; j < MAX_WIDTH; j++){

			double out_x = output[i*MAX_WIDTH+j].x;
			double out_y = output[i*MAX_WIDTH+j].y;

			if (out_x * out_x + out_y * out_y > 25.0){
				outFlow.Pixel(j, i, 0) = 1e10;
				outFlow.Pixel(j, i, 1) = 1e10;
			}
			else{
				outFlow.Pixel(j, i, 0) = out_x;
				outFlow.Pixel(j, i, 1) = out_y;
			}
		}
	}

	WriteFlowFile(outFlow, outFile.c_str());

	double accum_error = 0;
	int num_pix = 0;

	for (int i = 0; i < MAX_HEIGHT; i++){
		for (int j = 0; j < MAX_WIDTH; j++){
			double out_x = outFlow.Pixel(j, i, 0);
			double out_y = outFlow.Pixel(j, i, 1);

			if (unknown_flow(out_x, out_y)) continue;

			double out_deg = atan2(-out_y, -out_x) * 180.0 / M_PI;
			double ref_x = refFlow.Pixel(j, i, 0);
			double ref_y = refFlow.Pixel(j, i, 1);
			double ref_deg = atan2(-ref_y, -ref_x) * 180.0 / M_PI;

			//Normalize error to [-180, 180]
			double error = out_deg - ref_deg;
			while (error < -180) error += 360;
			while (error > 180) error -= 360;

			accum_error += fabs(error);
			num_pix++;
		}
	}

	double avg_error = accum_error / num_pix;

	std::cout << "Average error: " << avg_error << " degrees" << std::endl;

}

void OpticalFlow_output(){

        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;

   	std::string outFile = pwd + "/applications/optflow_dataset/current/output.flow";
	std::string reference_file = pwd + "/applications/optflow_dataset/current/ref.flo";

	velocity_t *outputs = (velocity_t*)malloc(sizeof(velocity_t) * MAX_HEIGHT * MAX_WIDTH);
   	read_fpga(outputs, sizeof(velocity_t)*MAX_HEIGHT*MAX_WIDTH, OPTICALFLOW_OUTPUT);

   	CFloatImage refFlow;
	ReadFlowFile(refFlow, reference_file.c_str()); 

  	check_results(outputs, refFlow, outFile);

  	free(outputs);

}

