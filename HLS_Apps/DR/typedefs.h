/*===============================================================*/
/*                                                               */
/*                          typedefs.h                           */
/*                                                               */
/*           Constant definitions and typedefs for host.         */
/*                                                               */
/*===============================================================*/

#ifndef __TYPEDEFS_H__
#define __TYPEDEFS_H__

//#define APP
#define DATAFLOW
//#define NONDATAFLOW
//#define SW

// dataset information
#define NUM_TRAINING 18000
#define CLASS_SIZE 1800
#define NUM_TEST 2000
#define DIGIT_WIDTH 4

#include <stdint.h>

// typedefs
typedef unsigned long long DigitType;
typedef unsigned char      LabelType;

#ifndef SW
  #include "ap_int.h"
  // sdsoc wide vector type
  typedef ap_uint<256>  WholeDigitType;
#endif

// parameters
#define K_CONST 3
#define PAR_FACTOR 10


#ifdef APP
void DigitRec(WholeDigitType global_training_set[NUM_TRAINING / 2], WholeDigitType global_test_set[NUM_TEST], LabelType global_results[NUM_TEST], int run);
#endif


#include <hls_stream.h>
//STREAM FOR 256 BITS
typedef ap_uint<256> bit256;
typedef hls::stream<bit256> stream256;

#ifdef DATAFLOW


void train_module (WholeDigitType *test_inst, WholeDigitType *train_inst, stream256 &knn_set);
void predict_module(stream256 &knn_set, uint32_t *global_results);
#endif

#ifdef NONDATAFLOW

typedef struct{
	int arr[K_CONST];
}int3_arr;

void train_module(WholeDigitType *test_inst, WholeDigitType *train_inst, int3_arr *knn_set);
void predict_module(int3_arr *knn_set, uint32_t *global_results);
#endif

#endif
