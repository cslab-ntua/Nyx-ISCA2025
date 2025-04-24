/*
<Lenet Neural Network High Level Synthesis Lenet>

MIT License

Copyright (c) 2020 Filipe Maciel Lins

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions: The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "lenet_host.h"

#define NUM_IMAGES 10

int main(){


	//hls::stream<float> fc3_out("fc3_out");
	int size_output = FC3_ACT_SIZE;
	float fc3_out_layer[size_output];

	int i,j,k;

	// read file with test results and store in array
	int fc_layer3_ref[10000];
	// open as cpp
	ifstream ifs("./test_result.out");
	if (!ifs.is_open()){
		cout << "The filename test is unable to open! " << endl;
		return 1;
	}
	i = 0;
	while(ifs){
		ifs >> fc_layer3_ref[i];
		i++;
	}
	ifs.close();

	float image[IMAGE_SIZE*IMAGE_SIZE*IMAGE_CHANNELS];
	float sti_val;
	string stimulus_source;



	ifstream myfile ("./filenames.out");
	if (!myfile.is_open()){
		cout << "The filename filenames is unable to open! " << endl;
		return 1;
	}

	float wrong_val = 0, corrected_val = 0;

	for (int j = 0; j < NUM_IMAGES; j++){
		//hls::stream<float> image_input("image_input");
		getline(myfile, stimulus_source);

		//cout << stimulus_source << endl;
		ifstream sti (stimulus_source);
		if (!sti.is_open()){
			cout << "The image file is unable to open! " << stimulus_source << endl;
			return 1;
		}
		else{
			k = 0;
			while (sti >> sti_val){ // sti_val is now a normal integer, so it'll be fine.
				image[k] = sti_val; // Conversion to ap_int<8> occurs automatically; you may want to check bounds.
				k++;
			}
		}
		sti.close();

		#ifdef DATAFLOW
		stream256 conv1_out("conv1_out");
		stream256 pool1_out("pool1_out");
		stream256 conv2_out("conv2_out");
		stream256 pool2_out("pool2_out");
		stream256 fc1_out("fc1_out");
		stream256 fc2_out("fc2_out");

		conv1(image, conv1_out);
		pool1(conv1_out, pool1_out);
		conv2(pool1_out, conv2_out);
		pool2(conv2_out, pool2_out);
		fc1(pool2_out, fc1_out);
		fc2(fc1_out, fc2_out);
		fc3(fc2_out, fc3_out_layer);
		#endif

		#ifdef NONDATAFLOW
		float* conv1_out = new float[A1_CHANNELS * POOL1_BUFFER_SIZE];
		float *pool1_out = new float[CONV2_BUFFER_SIZE];
		float *conv2_out = new float[A2_CHANNELS * POOL2_BUFFER_SIZE];
		float *pool2_out = new float[P2_CHANNELS * P2_SIZE * P2_SIZE];
		float *fc1_out = new float[FC2_WEIGHTS_H];
		float *fc2_out = new float[FC3_WEIGHTS_H];

		conv1(image, conv1_out);
		pool1(conv1_out, pool1_out);
		conv2(pool1_out, conv2_out);
		pool2(conv2_out, pool2_out);
		fc1(pool2_out, fc1_out);
		fc2(fc1_out, fc2_out);
		fc3(fc2_out, fc3_out_layer);

		delete []conv1_out;
		delete []pool1_out;
		delete []conv2_out;
		delete []pool2_out;
		delete []fc1_out;
		delete []fc2_out;
		#endif

		// one hot enconding
		float max_val = fc3_out_layer[0];
		int max = 0;

		for (i = 0; i < size_output; i++){
			if (fc3_out_layer[i] > max_val) {
				max_val = fc3_out_layer[i];
				max = i;
			}
		}

		cout << max << " " << fc_layer3_ref[j] << endl;
		if(max == fc_layer3_ref[j]){
			corrected_val++;}
		else{
			wrong_val++;
		}
	}
	cout << "corrected values: " << corrected_val/NUM_IMAGES << endl;
	cout << "wrong values: " << wrong_val/NUM_IMAGES << endl;

	return 0;

}
