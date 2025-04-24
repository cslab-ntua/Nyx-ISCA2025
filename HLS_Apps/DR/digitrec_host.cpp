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

// other headers
#include "typedefs.h"
#include "check_result.h"
#include "digitrec_sw.h"

// data
#include "digit_rec_data.h"

int main(int argc, char ** argv){

	printf("Digit Recognition Application\n");
	// for this benchmark, data is already included in arrays:
	//   training_data: contains 18000 training samples, with 1800 samples for each digit class
	//   testing_data:  contains 2000 test samples
	//   expected:      contains labels for the test samples

  // opencl version host code
  	#ifdef DATAFLOW
    // create space for the result
    LabelType* result = new LabelType[NUM_TEST];
    uint32_t * result_ = new uint32_t[NUM_TEST];

    WholeDigitType* test_in = new WholeDigitType[NUM_TEST];
    WholeDigitType* train_in = new WholeDigitType[NUM_TRAINING];

    for(int i=0; i<NUM_TEST; i++){
    	test_in[i].range(63 , 0  ) = testing_data[i*DIGIT_WIDTH+0];
    	test_in[i].range(127, 64 ) = testing_data[i*DIGIT_WIDTH+1];
    	test_in[i].range(191, 128) = testing_data[i*DIGIT_WIDTH+2];
    	test_in[i].range(255, 192) = testing_data[i*DIGIT_WIDTH+3];
    }

    for(int i=0; i<NUM_TRAINING; i++){
    	train_in[i].range(63 , 0  ) = training_data[i*DIGIT_WIDTH+0];
    	train_in[i].range(127, 64 ) = training_data[i*DIGIT_WIDTH+1];
    	train_in[i].range(191, 128) = training_data[i*DIGIT_WIDTH+2];
    	train_in[i].range(255, 192) = training_data[i*DIGIT_WIDTH+3];
    }

    //stream256 testing_set("testing_set");
    //stream256 training_set("training_set");
    //init_module(test_in, train_in, testing_set, training_set);
    stream256 knn_set("knn_set");
    train_module (test_in, train_in, knn_set);
    printf("Checking results: \t %d \n", knn_set.size());

    //train_module (testing_set, training_set, knn_set);
    predict_module(knn_set, result_);

    for(int i=0; i <NUM_TEST; i++){
    	result[i] = (LabelType)result_[i];
    }

  	#endif

	#ifdef NONDATAFLOW
    // create space for the result
    LabelType* result = new LabelType[NUM_TEST];
    uint32_t * result_ = new uint32_t[NUM_TEST];
    WholeDigitType* test_in = new WholeDigitType[NUM_TEST];
    WholeDigitType* train_in = new WholeDigitType[NUM_TRAINING];

    for(int i=0; i<NUM_TEST; i++){
    	test_in[i].range(63 , 0  ) = testing_data[i*DIGIT_WIDTH+0];
    	test_in[i].range(127, 64 ) = testing_data[i*DIGIT_WIDTH+1];
    	test_in[i].range(191, 128) = testing_data[i*DIGIT_WIDTH+2];
    	test_in[i].range(255, 192) = testing_data[i*DIGIT_WIDTH+3];
    }

    for(int i=0; i<NUM_TRAINING; i++){
    	train_in[i].range(63 , 0  ) = training_data[i*DIGIT_WIDTH+0];
    	train_in[i].range(127, 64 ) = training_data[i*DIGIT_WIDTH+1];
    	train_in[i].range(191, 128) = training_data[i*DIGIT_WIDTH+2];
    	train_in[i].range(255, 192) = training_data[i*DIGIT_WIDTH+3];
    }

    int3_arr *knn_set = new int3_arr[NUM_TEST*PAR_FACTOR];

    train_module(test_in, train_in, knn_set);
    predict_module(knn_set, result_);
    delete []knn_set;

    for(int i=0; i <NUM_TEST; i++){
    	result[i] = (LabelType)result_[i];
    }

	#endif

    // sdsoc version host code
  	#ifdef APP
    // allocate space for hardware function
    WholeDigitType* training_in0 = (WholeDigitType*)malloc(sizeof(WholeDigitType) * NUM_TRAINING / 2);
    WholeDigitType* training_in1 = (WholeDigitType*)malloc(sizeof(WholeDigitType) * NUM_TRAINING / 2);
    WholeDigitType* test_in      = (WholeDigitType*)malloc(sizeof(WholeDigitType) * NUM_TEST);

    // pack the data into a wide datatype
    for(int i=0; i<NUM_TRAINING/2; i++){
    	training_in0[i].range(63 , 0  ) = training_data[i*DIGIT_WIDTH+0];
    	training_in0[i].range(127, 64 ) = training_data[i*DIGIT_WIDTH+1];
    	training_in0[i].range(191, 128) = training_data[i*DIGIT_WIDTH+2];
    	training_in0[i].range(255, 192) = training_data[i*DIGIT_WIDTH+3];
    }
    for(int i=0; i<NUM_TRAINING/2; i++){
    	training_in1[i].range(63 , 0  ) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+0];
    	training_in1[i].range(127, 64 ) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+1];
    	training_in1[i].range(191, 128) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+2];
    	training_in1[i].range(255, 192) = training_data[(NUM_TRAINING / 2 + i)*DIGIT_WIDTH+3];
    }
    for(int i=0; i<NUM_TEST; i++){
    	test_in[i].range(63 , 0  ) = testing_data[i*DIGIT_WIDTH+0];
    	test_in[i].range(127, 64 ) = testing_data[i*DIGIT_WIDTH+1];
    	test_in[i].range(191, 128) = testing_data[i*DIGIT_WIDTH+2];
    	test_in[i].range(255, 192) = testing_data[i*DIGIT_WIDTH+3];
    }

    // create space for result
    LabelType* result = (LabelType*)malloc(sizeof(LabelType) * NUM_TEST);

    // run the hardware function
    // first call: transfer data
    DigitRec(training_in0, test_in, result, 0);

    // second call: execute
    DigitRec(training_in1, test_in, result, 1);
  	#endif

    // sw version host code
  	#ifdef SW
    // create space for the result
    LabelType* result = new LabelType[NUM_TEST];
    // software version
    DigitRec_sw(training_data, testing_data, result);
  	#endif

    // check results
    printf("Checking results:\n");
    check_results(result, expected, NUM_TEST);

    // cleanup
  	#ifdef DATAFLOW
    delete []test_in;
    delete []train_in;
    delete []result;
  	#endif

	#ifdef NONDATAFLOW
	delete []test_in;
	delete []train_in;
	delete []result;
	#endif

  	#ifdef SW
    delete []result;
	#endif

	#ifdef APP
    free(training_in0);
    free(training_in1);
    free(test_in);
    free(result);
	#endif

    return EXIT_SUCCESS;
}
