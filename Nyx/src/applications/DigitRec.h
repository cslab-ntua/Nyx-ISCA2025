#ifndef _DIGITREC_H_
#define _DIGITREC_H_

#include <stdint.h>
#include <stdio.h>
#include "../fpga/fpga_utils.h"
#include <fstream>
#include "iostream"

#define DIGITREC_INPUT_TEST   0x0000000100010000
#define DIGITREC_INPUT_TRAIN 0x0000000000010000
#define DIGITREC_OUTPUT  0x0000000500010000

// dataset information
#define NUM_TRAINING 18000
#define CLASS_SIZE 1800
#define NUM_TEST 2000
#define DIGIT_WIDTH 4

// typedefs
typedef unsigned long long DigitType;
typedef unsigned char      LabelType;

const DigitType training_data[NUM_TRAINING * DIGIT_WIDTH] = {
  #include "./196data/training_set_0.dat"
  #include "./196data/training_set_1.dat"
  #include "./196data/training_set_2.dat"
  #include "./196data/training_set_3.dat"
  #include "./196data/training_set_4.dat"
  #include "./196data/training_set_5.dat"
  #include "./196data/training_set_6.dat"
  #include "./196data/training_set_7.dat"
  #include "./196data/training_set_8.dat"
  #include "./196data/training_set_9.dat"
};

const DigitType testing_data[NUM_TEST * DIGIT_WIDTH] = {
  #include "./196data/test_set.dat"
};

const LabelType expected[NUM_TEST] = {
  #include "./196data/expected.dat"
};

void DigitRec_input();
void DigitRec_output();


#endif
