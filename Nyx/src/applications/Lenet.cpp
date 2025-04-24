#include "Lenet.h"

#include <iostream>

#define NUM_IMAGES 1

void Lenet_input(){

	float *image = (float*)malloc(sizeof(float) * IMAGE_CHANNELS * IMAGE_SIZE * IMAGE_SIZE);
	string stimulus_source;
	float sti_val;

        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
        ifstream myfile (pwd + "/applications/filenames.out");
	if (!myfile.is_open()){
		cout << "The filename filenames is unable to open! " << endl;
		return;
	}

	getline(myfile, stimulus_source);

        ifstream sti (pwd + "/applications/" + stimulus_source);
	if (!sti.is_open()){
		cout << "The image file is unable to open! " << stimulus_source << endl;
		return;
	}
	else{
		int k = 0;
		while (sti >> sti_val){ // sti_val is now a normal integer, so it'll be fine.
			image[k] = sti_val; // Conversion to ap_int<8> occurs automatically; you may want to check bounds.
			k++;
		}
	}
	sti.close();

	write_fpga(image, sizeof(float) * IMAGE_CHANNELS * IMAGE_SIZE * IMAGE_SIZE, LENET_INPUT);

	free(image);

}


void Lenet_output(){


	int size_output = FC3_ACT_SIZE;

	int *fc_layer3_ref = (int*)malloc(sizeof(int) * 10000);
	float *fc3_out_layer = (float*)malloc(sizeof(float) * size_output);
	float wrong_val = 0, corrected_val = 0;

        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
        ifstream ifs(pwd + "/applications/test_result.out");
	if (!ifs.is_open()){
		cout << "The filename test is unable to open! " << endl;
		return;
	}
	int i = 0;
	while(ifs){
		ifs >> fc_layer3_ref[i];
		i++;
	}
	ifs.close();


   	read_fpga(fc3_out_layer, sizeof(float) * size_output, LENET_OUTPUT);

	// one hot enconding
	float max_val = fc3_out_layer[0];
	int max = 0;

	for (int i = 0; i < size_output; i++){
		if (fc3_out_layer[i] > max_val) {
			max_val = fc3_out_layer[i];
			max = i;
		}
	}

	cout << max << " " << fc_layer3_ref[0] << endl;
	if(max == fc_layer3_ref[0]){
		corrected_val++;}
	else{
		wrong_val++;
	}

	cout << "corrected values: " << corrected_val/NUM_IMAGES << endl;
	cout << "wrong values: " << wrong_val/NUM_IMAGES << endl;

	free(fc_layer3_ref);
	free(fc3_out_layer);


}
