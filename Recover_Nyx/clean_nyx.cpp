/*===============================================================*/
/*                                                               */
/*                    modified_axpy.cpp                      	 */
/*                                                               */
/*     Main host function for the Modified AXPY application.     */
/*                                                               */
/*===============================================================*/



// standard C/C++ headers
#include <cstdio>
#include <cstdlib>
#include <getopt.h>
#include <string>
#include <iostream>
#include <chrono>

// other headers
#include "fpga/fpga.h"

using namespace std::chrono;

int main(int argc, char **argv){

	std::cout << "Clean Nyx" << std::endl << std::endl;

	int lite_restart[1] = {0x00000081};

	/////////////////////////////////////////////////

  	Program_Device("./bitstreams/recover_PRR0.bin",0);  // ********************* Reconfiguration of PRR 0 **********************
	write_to_device(lite_restart, sizeof(int), 0x0000004000000000); // Clean PRR 0

 	Program_Device("./bitstreams/recover_PRR1.bin",1); // ********************** Reconfiguration of PRR 1 **********************
  	write_to_device(lite_restart, sizeof(int), 0x0000004000010000); // Clean PRR 1

 	Program_Device("./bitstreams/recover_PRR2.bin",2); // ********************** Reconfiguration of PRR 2 **********************
  	write_to_device(lite_restart, sizeof(int), 0x0000004000020000); // Clean PRR 2 

        Program_Device("./bitstreams/recover_PRR3.bin",3); // ********************** Reconfiguration of PRR 3 **********************
        write_to_device(lite_restart, sizeof(int), 0x0000004000030000); // Clean PRR 3

	Program_Device("./bitstreams/recover_PRR4.bin",4); // ********************** Reconfiguration of PRR 4 **********************
        write_to_device(lite_restart, sizeof(int), 0x0000004000040000); // Clean PRR 4

	Program_Device("./bitstreams/recover_PRR5.bin",5); // ********************** Reconfiguration of PRR 5 **********************
        write_to_device(lite_restart, sizeof(int), 0x0000004000050000); // Clean PRR 5

	Program_Device("./bitstreams/recover_PRR6.bin",6); // ********************** Reconfiguration of PRR 6 **********************
        write_to_device(lite_restart, sizeof(int), 0x0000004000060000); // Clean PRR 6

	Program_Device("./bitstreams/recover_PRR7.bin",7); // ********************** Reconfiguration of PRR 7 **********************
        write_to_device(lite_restart, sizeof(int), 0x0000004000070000); // Clean PRR 7


	sleep(2);

	int lite_start[1] = {0x00000001};

        /////////////////////////////////////////////////

        Program_Device("./bitstreams/recover_PRR0.bin",0);  // ********************* Reconfiguration of PRR 0 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000000000); // Clean PRR 0

        Program_Device("./bitstreams/recover_PRR1.bin",1); // ********************** Reconfiguration of PRR 1 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000010000); // Clean PRR 1

        Program_Device("./bitstreams/recover_PRR2.bin",2); // ********************** Reconfiguration of PRR 2 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000020000); // Clean PRR 2

        Program_Device("./bitstreams/recover_PRR3.bin",3); // ********************** Reconfiguration of PRR 3 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000030000); // Clean PRR 3

        Program_Device("./bitstreams/recover_PRR4.bin",4); // ********************** Reconfiguration of PRR 4 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000040000); // Clean PRR 4

        Program_Device("./bitstreams/recover_PRR5.bin",5); // ********************** Reconfiguration of PRR 5 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000050000); // Clean PRR 5

        Program_Device("./bitstreams/recover_PRR6.bin",6); // ********************** Reconfiguration of PRR 6 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000060000); // Clean PRR 6

        Program_Device("./bitstreams/recover_PRR7.bin",7); // ********************** Reconfiguration of PRR 7 **********************
        write_to_device(lite_start, sizeof(int), 0x0000004000070000); // Clean PRR 7


  	return EXIT_SUCCESS;

}
