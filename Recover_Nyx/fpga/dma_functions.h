/*===============================================================*/
/*                                                               */
/*                        dma_functions.h                        */
/*                                                               */
/*      DMA Functions to Communication with the FPGA device      */
/*                                                               */
/*===============================================================*/


#ifndef DMA_FUNCTIONS_HPP_
#define DMA_FUNCTIONS_HPP_

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

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

//Function to read from FPGA memory through XDMA
int read_from_device(void *var_ptr, uint64_t size, uint64_t address);

//Function to write to FPGA memory through XDMA
int write_to_device(void *var_ptr, uint64_t size, uint64_t address);

//Function to reconfigure PR regions through ICAP
int write_bitstream(char *bit_ptr, uint64_t size, uint64_t address);

//Read from output file (FPGA) to a buffer (Host PTR) 
ssize_t read_to_buffer(char *fname, int fd, char *buffer, uint64_t size, uint64_t base);

//Write from buffer (Host PTR) to output file (FPGA)
ssize_t write_from_buffer(char *fname, int fd, char *buffer, uint64_t size, uint64_t base);

//Write from buffer (Host PTR) to output file (FPGA) for Partial Reconfiguration 
ssize_t write_from_buffer_bit(char *fname, int fd, char *buffer, uint64_t size, uint64_t base);

#endif