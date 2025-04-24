/*===============================================================*/
/*                                                               */
/*                    dma_write.cpp                      */
/*                                                               */
/*      Function to write to fpga address space.     */
/*                                                               */
/*===============================================================*/


#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>

#include "dma_functions.h"

/*
 * man 2 write:
 * On Linux, write() (and similar system calls) will transfer at most
 *      0x7ffff000 (2,147,479,552) bytes, returning the number of bytes
 *      actually transferred.  (This is true on both 32-bit and 64-bit
 *      systems.)
 */

#define RW_MAX_SIZE     0x7ffff000
//#define RW_MAX_SIZE 0x4
int verbose = 0;


uint64_t getopt_integer(char *optarg){

	int rc;
    uint64_t value;

    rc = sscanf(optarg, "0x%lx", &value);
    if (rc <= 0)
    	rc = sscanf(optarg, "%lu", &value);

    return value;
}


ssize_t read_to_buffer(char *fname, int fd, char *buffer, uint64_t size, uint64_t base){
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;
	int loop = 0;

	while (count < size){
		uint64_t bytes = size - count;

        if (bytes > RW_MAX_SIZE)
        	bytes = RW_MAX_SIZE;

    	if (offset){
			rc = lseek(fd, offset, SEEK_SET);
			if (rc != offset){
				fprintf(stderr, "%s, seek off 0x%lx != 0x%lx.\n", fname, rc, offset);
				perror("seek file");
				return -EIO;
			}
		}

		/* read data from file into memory buffer */
		rc = read(fd, buf, bytes);
		if (rc < 0){
			fprintf(stderr, "%s, read 0x%lx @ 0x%lx failed %ld.\n", fname, bytes, offset, rc);
			perror("read file");
			return -EIO;
		}

		count += rc;
		if (rc != bytes){
			fprintf(stderr, "%s, read underflow 0x%lx/0x%lx @ 0x%lx.\n", fname, rc, bytes, offset);
			break;
		}

		buf += bytes;
		offset += bytes;
		loop++;
	}

    if (count != size && loop)
    	fprintf(stderr, "%s, read underflow 0x%lx/0x%lx.\n", fname, count, size);
        
	return count;
}


ssize_t write_from_buffer_bit(char *fname, int fd, char *buffer, uint64_t size, uint64_t base){
	
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;
	int loop = 0;

	while (count < size){
		uint64_t bytes = size - count;

		if (bytes > 1024)
			bytes = 1024;

		if (offset){
			rc = lseek(fd, offset, SEEK_SET);
			if (rc != offset){
				fprintf(stderr, "%s, seek off 0x%lx != 0x%lx.\n", fname, rc, offset);
                perror("seek file");
                return -EIO;
			}
		}

        /* write data to file from memory buffer */
		rc = write(fd, buf, bytes);
		if (rc < 0){
			fprintf(stderr, "%s, write 0x%lx @ 0x%lx failed %ld.\n", fname, bytes, offset, rc);
			perror("write file");
			return -EIO;
		}

        count += rc;
        if (rc != bytes){
			fprintf(stderr, "%s, write underflow 0x%lx/0x%lx @ 0x%lx.\n", fname, rc, bytes, offset);
				break;
		}
        
        buf += bytes;
		loop++;
	}

	if (count != size && loop)
    	fprintf(stderr, "%s, write underflow 0x%lx/0x%lx.\n", fname, count, size);


	return count;
}

ssize_t write_from_buffer(char *fname, int fd, char *buffer, uint64_t size, uint64_t base){
        
	ssize_t rc;
	uint64_t count = 0;
	char *buf = buffer;
	off_t offset = base;
	int loop = 0;

	while (count < size){
		uint64_t bytes = size - count;

		if (bytes > RW_MAX_SIZE)
			bytes = RW_MAX_SIZE;

		if (offset){
			rc = lseek(fd, offset, SEEK_SET);
			if (rc != offset){
				fprintf(stderr, "%s, seek off 0x%lx != 0x%lx.\n", fname, rc, offset);
				perror("seek file");
            	return -EIO;
        	}
		}

		/* write data to file from memory buffer */
    	rc = write(fd, buf, bytes);
    	if (rc < 0){
			fprintf(stderr, "%s, write 0x%lx @ 0x%lx failed %ld.\n", fname, bytes, offset, rc);
			perror("write file");
			return -EIO;
    	}

		count += rc;
		if (rc != bytes){
			fprintf(stderr, "%s, write underflow 0x%lx/0x%lx @ 0x%lx.\n", fname, rc, bytes, offset);
			break;
		}
        
        buf += bytes;
        offset += bytes;
		loop++;
	}

    if (count != size && loop)
		fprintf(stderr, "%s, write underflow 0x%lx/0x%lx.\n", fname, count, size);

	return count;
}






