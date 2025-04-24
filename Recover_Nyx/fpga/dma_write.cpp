/*===============================================================*/
/*                                                               */
/*                    dma_write.cpp                      */
/*                                                               */
/*      Function to write to fpga address space.     */
/*                                                               */
/*===============================================================*/


#define _BSD_SOURCE
#define _XOPEN_SOURCE 500
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
#include <iostream>
#include <cmath>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "dma_functions.h"

using namespace std;


static int dma_bitstream(char *devname,uint64_t addr, uint64_t aperture, uint64_t size, char *bit_ptr){

	uint64_t i;
	ssize_t rc;
    	size_t bytes_done = 0;
    	uint64_t apt_loop = aperture ? (size + aperture - 1) / aperture : 0;
    	char *buffer = NULL;
    	char *allocated = NULL;
    	int fpga_fd = open(devname, O_RDWR);
    	int underflow = 0;
	size_t page_size = sysconf(_SC_PAGESIZE);
	int bit_fd=-1;

	if (fpga_fd < 0) {
		fprintf(stderr, "unable to open device %s, %d.\n", devname, fpga_fd);
		perror("open device");
		return -EINVAL;
	}


	posix_memalign((void **)&allocated, page_size /*alignment */ , size + page_size);
        
	if (!allocated){
		fprintf(stderr, "OOM %lu.\n", size + page_size);
		rc = -ENOMEM;
		goto out;
	}
	buffer = allocated;
        

	if(bit_ptr){
		bit_fd = open (bit_ptr, O_RDONLY);
		if(bit_fd < 0){
			printf("Unable to open bitstream file");
			rc= -EINVAL;
			goto out;
		}
	}

	if(bit_fd>=0){
		rc = read_to_buffer(bit_ptr,bit_fd,buffer,size,0);
		if (rc<0 || rc < size)
			goto out;
	}

	/* write buffer to AXI MM address using SGDMA */
	if (apt_loop){
		//printf("Apt Loop\n");
		uint64_t j;
		uint64_t len = size;
		char *buf = buffer;
		bytes_done = 0;
                
		for (j = 0; j < apt_loop; j++, len -= aperture, buf += aperture){
                        
			uint64_t bytes = (len > aperture) ? aperture : len;
			rc = write_from_buffer_bit(devname, fpga_fd, buf, bytes, addr);

			if (rc < 0)
				goto out;

			bytes_done += rc;
			if (!underflow && rc < bytes)
				underflow = 1;
		}
	}
	else {
		//printf("No Apt Loop\n");
		rc = write_from_buffer_bit(devname, fpga_fd, buffer, size, addr);
		if (rc < 0)
			goto out;

		bytes_done = rc;
		if (!underflow && bytes_done < size)
			underflow = 1;
	}

out:
        close(fpga_fd);
        free(allocated);

        if (rc < 0)
			return rc;
        /* treat underflow as error */
        return underflow ? -EIO : 0;
}

static int dma_write(char *devname, uint64_t addr, uint64_t aperture, uint64_t size, void *var_ptr){

	uint64_t i;
	ssize_t rc;
	size_t bytes_done = 0;
	size_t out_offset = 0;
	uint64_t apt_loop = aperture ? (size + aperture - 1) / aperture : 0;
	char *buffer = NULL;
	char *allocated = NULL;
	int fpga_fd = open(devname, O_RDWR);
	int underflow = 0;
	size_t page_size = sysconf(_SC_PAGESIZE);


	if (fpga_fd < 0) {
		fprintf(stderr, "unable to open device %s, %d.\n", devname, fpga_fd);
		perror("open device");
		return -EINVAL;
	}

	posix_memalign((void **)&allocated, page_size , size + page_size);
	if (!allocated) {
		fprintf(stderr, "OOM %lu.\n", size + page_size);
		rc = -ENOMEM;
		goto out;
	}

	buffer = allocated;

	memcpy(buffer,var_ptr,size);

	/* write buffer to AXI MM address using SGDMA */
	if (apt_loop){
		//printf("Apt Loop\n");
		uint64_t j;
		uint64_t len = size;
	    char *buf = buffer;

		bytes_done = 0;
		for (j = 0; j < apt_loop; j++, len -= aperture, buf += aperture) {
			
			uint64_t bytes = (len > aperture) ? aperture : len;
			rc = write_from_buffer(devname, fpga_fd, buf, bytes, addr);
			if (rc < 0)
				goto out;

			bytes_done += rc;
			if (!underflow && rc < bytes)
				underflow = 1;
		}
	} 
	else {
		//printf("No Apt Loop\n");
		rc = write_from_buffer(devname, fpga_fd, buffer, size, addr);
		if (rc < 0)
			goto out;

		bytes_done = rc;
		if (!underflow && bytes_done < size)
			underflow = 1;
	}


out:
	close(fpga_fd);
	free(allocated);

	if (rc < 0)
		return rc;
	/* treat underflow as error */
	return underflow ? -EIO : 0;
}

int write_bitstream(char *bit_ptr, uint64_t size, uint64_t address){
        char *device[1] = {"/dev/xdma0_h2c_0"};
        uint64_t aperture = 0;

        return dma_bitstream(device[0], address, aperture, size, bit_ptr);
}


int write_to_device(void *var_ptr, uint64_t size, uint64_t address){
        char *device[1] = {"/dev/xdma0_h2c_0"};
        uint64_t aperture = 0;

        return dma_write(device[0], address, aperture, size, var_ptr);
}
