/*===============================================================*/
/*                                                               */
/*                        check_result.h                         */
/*                                                               */
/*      Compare result and expected label to get error rate      */
/*                                                               */
/*===============================================================*/

#ifndef CHECK_RESULT_H
#define CHECK_RESULT_H

#include "typedefs.h"

void check_results(LabelType* result, const LabelType* expected, int cnt);

#endif
