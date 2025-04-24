/*===============================================================*/
/*                                                               */
/*                    dma_read.cpp                      */
/*                                                               */
/*      Function to read from fpga address space.     */
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
#include <time.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "dma_functions.h"


static int dma_read(char *devname, uint64_t addr, uint64_t aperture, uint64_t size, void *var_ptr){
        ssize_t rc = 0;
        size_t bytes_done = 0;
        uint64_t i;
        uint64_t apt_loop = aperture ? (size + aperture - 1) / aperture : 0;
        char *buffer = NULL;
        char *allocated = NULL;
        int fpga_fd=open(devname, O_RDWR);
        int underflow = 0;
        size_t page_size = sysconf(_SC_PAGESIZE);

        if (fpga_fd < 0) {
                fprintf(stderr, "unable to open device %s, %d.\n", devname, fpga_fd);
                perror("open device");
                return -EINVAL;
        }

        posix_memalign((void **)&allocated, page_size, size + page_size);
        
        if (!allocated){
                fprintf(stderr, "OOM %lu.\n", size + page_size);
                rc = -ENOMEM;
                goto out;
        }

        buffer = allocated;

        if (apt_loop) {
		printf("Apt Loop\n");
			uint64_t j;
			uint64_t len = size;
			char *buf = (char*) buffer;

			bytes_done = 0;
            for (j = 0; j < apt_loop; j++, len -= aperture, buf += aperture) {
				uint64_t bytes = (len > aperture) ? aperture : len,
				rc = read_to_buffer(devname, fpga_fd, buf, bytes, addr);
				if (rc < 0)
					goto out;

				if (!underflow && rc < bytes)
					underflow = 1;
				
				bytes_done += rc;
			}
        } 
        else{
		//printf("No Apt Loop\n");
                rc = read_to_buffer(devname, fpga_fd, buffer, size, addr);
                if (rc < 0)
					goto out;
                
                bytes_done = rc;

                if (!underflow && bytes_done < size)
					underflow = 1;
        }

        memcpy(var_ptr,buffer,size);

        if (!underflow)
                rc = 0;
        else
                rc = -EIO;

out:
        close(fpga_fd);
        free(allocated);

        return rc;
}

int read_from_device(void *var_ptr, uint64_t size, uint64_t address){
        //char *device[4] = {"/dev/xdma0_c2h_0","/dev/xdma0_c2h_1","/dev/xdma0_c2h_2","/dev/xdma0_c2h_3"};
        char *device[1] = {"/dev/xdma0_c2h_0"};
        uint64_t aperture = 0;

        return dma_read(device[0], address, aperture, size, var_ptr);
}
