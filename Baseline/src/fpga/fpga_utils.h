/*===============================================================*/
/*                                                               */
/*                        dma_utils.h                        	 */
/*                                                               */
/*      DMA Functions to Communication with the FPGA device      */
/*                                                               */
/*===============================================================*/


#ifndef FPGA_UTILS_H
#define FPGA_UTILS_H

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include "../event_gen.h"

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

void read_fpga(void *var_ptr, uint64_t size, uint64_t addr); /* Read from FPGA address space */
void write_fpga(void *var_ptr, uint64_t size, uint64_t addr); /* Write to FPGA address space */
void write_bin(const char *bin_ptr, uint64_t size, uint64_t addr); /* Write to FPGA ICAP */

#endif
