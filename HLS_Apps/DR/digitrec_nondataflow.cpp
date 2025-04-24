/*===============================================================*/
/*                                                               */
/*                          digitrec.cpp                         */
/*                                                               */
/*             Hardware function for digit recognition           */
/*                                                               */
/*===============================================================*/

#include "typedefs.h"
#include <stdio.h>

#ifdef NONDATAFLOW

// popcount function
int popcount(WholeDigitType x){
  // most straightforward implementation
  // actually not bad on FPGA
  int cnt = 0;
  for (int i = 0; i < 256; i ++)
    cnt = cnt + x[i];

  return cnt;
}

// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.
//void update_knn( WholeDigitType test_inst, WholeDigitType train_inst, int min_distances[K_CONST] )
void update_knn(const WholeDigitType test_inst, const WholeDigitType train_inst, int min_distances[K_CONST]){

	//#pragma HLS INLINE

	// Compute the difference using XOR
	WholeDigitType diff = test_inst ^ train_inst;

	int dist = 0;

	dist = popcount(diff);

	int max_dist = 0;
	int max_dist_id = K_CONST+1;
	int k = 0;

	// Find the max distance
	FIND_MAX_DIST: for (int k=0; k<K_CONST; ++k){
		if (min_distances[k] > max_dist){
			max_dist = min_distances[k];
			max_dist_id = k;
		}
	}

	// Replace the entry with the max distance
	if ( dist < max_dist )
		min_distances[max_dist_id] = dist;
}

// Given 10xK minimum distance values, this function
// finds the actual K nearest neighbors and determines the
// final output based on the most common int represented by
// these nearest neighbors (i.e., a vote among KNNs).
LabelType knn_vote( int knn_set[PAR_FACTOR * K_CONST]){

	//#pragma HLS INLINE

	// final K nearest neighbors
	int min_distance_list[K_CONST];
  	#pragma HLS array_partition variable=min_distance_list complete dim=0
	// labels for the K nearest neighbors
	int label_list[K_CONST];
  	#pragma HLS array_partition variable=label_list complete dim=0
	// voting boxes
	int vote_list[10];
  	#pragma HLS array_partition variable=vote_list complete dim=0

	int pos = 1000;

	// initialize
	INIT_1: for (int i=0; i<K_CONST; i++){
    	#pragma HLS unroll
		min_distance_list[i] = 256;
		label_list[i] = 9;
	}

	INIT_2: for (int i=0; i<10; i++){
    	#pragma HLS unroll
		vote_list[i] = 0;
	}

	// go through all the lanes
	// do an insertion sort to keep a sorted neighbor list
	LANES: for (int i = 0; i<PAR_FACTOR; i++){
		INSERTION_SORT_OUTER: for (int j = 0; j < K_CONST; j++){
      	  #pragma HLS pipeline II=1
			pos = 1000;
			INSERTION_SORT_INNER: for (int r = 0; r<K_CONST; r++){
        		#pragma HLS unroll
				pos = ((knn_set[i*K_CONST+j] < min_distance_list[r]) && (pos > K_CONST)) ? r : pos;
			}

			INSERT: for (int r=K_CONST; r>0; r--){
        		#pragma HLS unroll
				if(r-1 > pos){
					min_distance_list[r-1] = min_distance_list[r-2];
					label_list[r-1] = label_list[r-2];
				}
				else if (r-1 == pos){
					min_distance_list[r-1] = knn_set[i*K_CONST+j];
					label_list[r-1] = i / (PAR_FACTOR / 10);
				}
			}
		}
	}

	// vote
	INCREMENT: for (int i=0; i<K_CONST; i++){
    	#pragma HLS pipeline II=1
		vote_list[label_list[i]] += 1;
	}

	LabelType max_vote;
	max_vote = 0;

	// find the maximum value
	VOTE: for (int i=0; i<10; i++){
    	#pragma HLS unroll
		if(vote_list[i] >= vote_list[max_vote]){
			max_vote = i;
		}
	}

	return max_vote;
}

void train_module(WholeDigitType *test_inst, WholeDigitType *train_inst, int3_arr *knn_set){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2000 port=test_inst offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=18000 port=train_inst offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=20000 port=knn_set offset=slave
	#pragma HLS INTERFACE s_axilite port=test_inst bundle=control
	#pragma HLS INTERFACE s_axilite port=train_inst bundle=control
	#pragma HLS INTERFACE s_axilite port=knn_set bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	int knn_set_tmp[PAR_FACTOR * K_CONST];
	#pragma HLS array_partition variable=knn_set_tmp complete dim=0

	// loop through test set
	TEST_LOOP: for(int t=0; t<NUM_TEST; ++t){
		TRAINING_LOOP : for ( int i=0; i<NUM_TRAINING / PAR_FACTOR; ++i){
			LANES : for ( int j=0; j<PAR_FACTOR; j++){
				#pragma HLS pipeline II=1
				if(i==0){
					for ( int r=0; r<K_CONST * PAR_FACTOR ; ++r){
					  #pragma HLS unroll
					  knn_set_tmp[r] = 256;
					}
				}

				update_knn(test_inst[t], train_inst[(j * NUM_TRAINING / PAR_FACTOR + i)], &knn_set_tmp[j*K_CONST]);
				if(i==NUM_TRAINING / PAR_FACTOR-1){
					int3_arr tmp_;
					tmp_.arr[0] = knn_set_tmp[j*K_CONST + 0];
					tmp_.arr[1] = knn_set_tmp[j*K_CONST + 1];
					tmp_.arr[2] = knn_set_tmp[j*K_CONST + 2];
					knn_set[t*PAR_FACTOR + j] = tmp_;
				}
			}
		}
	}
}

void predict_module(int3_arr *knn_set, uint32_t *global_results){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2000 port=global_results offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=20000 port=knn_set offset=slave
	#pragma HLS INTERFACE s_axilite port=global_results bundle=control
	#pragma HLS INTERFACE s_axilite port=knn_set bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	//local buffer
	static LabelType results[NUM_TEST];
	int knn_set_tmp[PAR_FACTOR * K_CONST];
	#pragma HLS array_partition variable=knn_set_tmp cyclic dim=0 factor=K_CONST


	// loop through test set
	TEST_LOOP: for (int t=0; t<NUM_TEST; ++t){

		STREAM_LOCAL: for(int iter=0; iter<PAR_FACTOR; iter++){
			#pragma HLS pipeline II=1
			int3_arr tmp_  = knn_set[t*PAR_FACTOR + iter];
			knn_set_tmp[iter*K_CONST + 0] = tmp_.arr[0];
			knn_set_tmp[iter*K_CONST + 1] = tmp_.arr[1];
			knn_set_tmp[iter*K_CONST + 2] = tmp_.arr[2];
		}

		// Compute the final output
		LabelType max_vote = knn_vote(knn_set_tmp);
		global_results[t] = (uint32_t)max_vote;
		//results[t] = max_vote;
	}

	// copy the results out
	//RESULTS_OUT: for (int i=0; i<NUM_TEST; i++){
	//	#pragma HLS pipeline II=1
	//	global_results[i] = (uint32_t)results[i];
	//}
}

#endif
