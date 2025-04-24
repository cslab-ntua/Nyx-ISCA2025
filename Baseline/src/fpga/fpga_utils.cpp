/*===============================================================*/
/*                                                               */
/*                    dma_write.cpp                      		 */
/*                                                               */
/*      Function to write to fpga address space.     			 */
/*                                                               */
/*===============================================================*/

#include "fpga_utils.h"

/*
 * man 2 write:
 * On Linux, write() (and similar system calls) will transfer at most
 *      0x7ffff000 (2,147,479,552) bytes, returning the number of bytes
 *      actually transferred.  (This is true on both 32-bit and 64-bit
 *      systems.)
 */

#define RW_MAX_SIZE     0x7ffff000

ssize_t dma_read(int fd, char *buffer, uint64_t size, uint64_t base){
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;

	while (count < size){
        	
		uint64_t bytes = size - count;
		if (bytes > RW_MAX_SIZE)
			bytes = RW_MAX_SIZE;

		if (offset){
			rc = lseek(fd, offset, SEEK_SET);
			assert(rc==offset);
		}

		/* read data from file into memory buffer */
		rc = read(fd, buf, bytes);    
		assert(rc>=0);
		
		count += rc;
		/*buf += bytes;
		offset += bytes;*/
		buf += rc;
		offset += rc;
	}
	return count;
}

ssize_t dma_bin(int fd, char *buffer, uint64_t size, uint64_t base){
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;

	while (count < size){
		
		uint64_t bytes = size - count;
		if (bytes > RW_MAX_SIZE)
			bytes = RW_MAX_SIZE;

		if (offset){
			rc = lseek(fd, offset, SEEK_SET);
			assert(rc==offset);
		}

        /* write data to file from memory buffer */
		rc = write(fd, buf, bytes);
		assert(rc>=0);
        count += rc;
        //buf += bytes;
		buf += rc;
	}
	return count;
}

ssize_t dma_write(int fd, char *buffer, uint64_t size, uint64_t base){

	ssize_t rc;
    uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;

	while (count < size){
		
		uint64_t bytes = size - count;
		if (bytes > RW_MAX_SIZE)
			bytes = RW_MAX_SIZE;

		if (offset){
			rc = lseek(fd, offset, SEEK_SET);
			assert(rc==offset);
		}

		/* write data to file from memory buffer */
        rc = write(fd, buf, bytes);
        assert(rc>=0);
		count += rc;
        /*buf += bytes;
        offset += bytes;*/
		buf += rc;
		offset += rc;
	}
	return count;
}

const char *C2H[4] = {"/dev/xdma0_c2h_0","/dev/xdma0_c2h_1","/dev/xdma0_c2h_2","/dev/xdma0_c2h_3"};

void read_fpga(void *var_ptr, uint64_t size, uint64_t addr){
	char *buffer = NULL;
	char *allocated = NULL;
	int fpga_fd=open(C2H[0], O_RDWR);
	size_t page_size = sysconf(_SC_PAGESIZE);

	assert(fpga_fd>=0);
	posix_memalign((void **)&allocated, page_size, size + page_size);
	assert(allocated!=NULL);
        
	buffer = allocated;
	dma_read(fpga_fd, buffer, size, addr);

	memcpy(var_ptr,buffer,size);
	close(fpga_fd);
	free(allocated);

}

const char *H2C[4] = {"/dev/xdma0_h2c_0","/dev/xdma0_h2c_1","/dev/xdma0_h2c_2","/dev/xdma0_h2c_3"};

void write_bin(const char *bin_ptr, uint64_t size, uint64_t addr){

	char *buffer = NULL;
	char *allocated = NULL;
	int fpga_fd = open(H2C[0], O_RDWR);
	size_t page_size = sysconf(_SC_PAGESIZE);
	int bin_fd = open(bin_ptr, O_RDONLY);

	assert(fpga_fd>=0);
	assert(bin_fd>=0);
	posix_memalign((void **)&allocated, page_size, size + page_size);
	assert(allocated!=NULL);

    buffer = allocated;

	//std::chrono::high_resolution_clock::time_point point1 = timestamp();
	dma_read(bin_fd,buffer,size,0);
	//std::chrono::high_resolution_clock::time_point point2 = timestamp();

	/* write buffer to AXI MM address using SGDMA */
	//std::chrono::high_resolution_clock::time_point point3 = timestamp();
	dma_bin(fpga_fd, buffer, size, addr);
	//std::chrono::high_resolution_clock::time_point point4 = timestamp();

	//std::cout << " Read Bitstream duration: " << duration(point1, point2) << std::endl;
	//std::cout << " Bitstream to FPGA duration " << duration(point3, point4) << std::endl;

	close(fpga_fd);
	close(bin_fd);
	free(allocated);

}

void write_fpga(void *var_ptr, uint64_t size, uint64_t addr){

	char *buffer = NULL;
	char *allocated = NULL;
	int fpga_fd = open(H2C[0], O_RDWR);
	size_t page_size = sysconf(_SC_PAGESIZE);

	assert(fpga_fd>=0);
	posix_memalign((void **)&allocated, page_size , size + page_size);
	assert(allocated!=NULL);
        
	buffer = allocated;
	memcpy(buffer,var_ptr,size);
	dma_write(fpga_fd, buffer, size, addr);

	close(fpga_fd);
	free(allocated);

}





