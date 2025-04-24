/*===============================================================*/
/*                                                               */
/*                       training_data.h                         */
/*                                                               */
/*              Constant array for training instances.           */
/*                                                               */
/*===============================================================*/


#ifndef DIGIT_REC_DATA_H
#define DIGIT_REC_DATA_H

#include "typedefs.h"


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

#endif
