#ifndef _SCALING_H_
#define _SCALING_H_

#include <stdint.h>
#include <stdio.h>
#include "../fpga/fpga_utils.h"

#define SCALING_INPUT   0x0000000D40010000
#define SCALING_OUTPUT  0x00000005C0010000

void Scaling_input();
void Scaling_output();


#endif