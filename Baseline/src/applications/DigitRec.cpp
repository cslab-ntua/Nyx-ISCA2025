#include "DigitRec.h"

void DigitRec_input(){


	DigitType *test_in = (DigitType*)malloc(sizeof(DigitType) * NUM_TEST * 4);
	DigitType *train_in = (DigitType*)malloc(sizeof(DigitType) * NUM_TRAINING * 4);
	
	for(int i=0; i<NUM_TEST; i++){
    	test_in[i*DIGIT_WIDTH+0] = testing_data[i*DIGIT_WIDTH+0];
    	test_in[i*DIGIT_WIDTH+1] = testing_data[i*DIGIT_WIDTH+1];
    	test_in[i*DIGIT_WIDTH+2] = testing_data[i*DIGIT_WIDTH+2];
    	test_in[i*DIGIT_WIDTH+3] = testing_data[i*DIGIT_WIDTH+3];
    }

    for(int i=0; i<NUM_TRAINING; i++){
    	train_in[i*DIGIT_WIDTH+0] = training_data[i*DIGIT_WIDTH+0];
    	train_in[i*DIGIT_WIDTH+1] = training_data[i*DIGIT_WIDTH+1];
    	train_in[i*DIGIT_WIDTH+2] = training_data[i*DIGIT_WIDTH+2];
    	train_in[i*DIGIT_WIDTH+3] = training_data[i*DIGIT_WIDTH+3];
    }

   	write_fpga(test_in, sizeof(DigitType) * 4 * NUM_TEST, DIGITREC_INPUT_TEST);
	write_fpga(train_in, sizeof(DigitType) * 4 * NUM_TRAINING, DIGITREC_INPUT_TRAIN);

    free(test_in);
    free(train_in);

}


void check_results(LabelType* result, const LabelType* expected, int cnt){

        int correct_cnt = 0;
        std::ofstream ofile;

        const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
        ofile.open(pwd + "/applications/outputs_digitrec.txt");

	if(ofile.is_open()){

		for (int i=0; i<cnt; i++){
			if(result[i] != expected[i])
				ofile << "Test " << i << ": expected = " << int(expected[i]) << ", result = " << int(result[i]) << std::endl;
			else
				correct_cnt ++;
		}

		ofile << "\n\t " << correct_cnt << " / " << cnt << " correct!" << std::endl;
		ofile.close();
	}
	else
		std::cout << "Failed to create output file!" << std::endl;
}


void DigitRec_output(){

	uint32_t *result_ = (uint32_t*)malloc(sizeof(uint32_t) * NUM_TEST);
	LabelType *result = (LabelType*)malloc(sizeof(LabelType) * NUM_TEST);

   	read_fpga(result_, sizeof(uint32_t)*NUM_TEST, DIGITREC_INPUT_TEST);

    for(int i=0; i <NUM_TEST; i++){
    	result[i] = (LabelType)result_[i];
    }

    printf("Checking results:\n");
    check_results(result, expected, NUM_TEST);

	free(result_);
	free(result);

}
