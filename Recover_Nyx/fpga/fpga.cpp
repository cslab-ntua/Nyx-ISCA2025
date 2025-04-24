/*===============================================================*/
/*                                                               */
/*                    fpga.cpp                      */
/*                                                               */
/*      Functions to interact with the FPGA device.     */
/*                                                               */
/*===============================================================*/


#include "fpga.h"
#include <iostream>
#include <fstream>

using namespace std;

uint8_t bitExtracted(uint32_t number, int k, int p){
    return (((1 << k) - 1) & (number >> (p - 1)));
}


void Program_Device(char* binFile, int PRR_Slot){

	ifstream in_file(binFile,ios::binary);
	in_file.seekg(0,ios::end);
	int bin_size[1]={in_file.tellg()/4};
	int status[1];
	int slot[1]={0x1};

	if(PRR_Slot==0){
		write_to_device(slot, sizeof(int), 0x0000004000100000);
   	}
        else if(PRR_Slot==1){
    		write_to_device(slot, sizeof(int), 0x0000004000110000);
        }
	else if(PRR_Slot==2){	
    		write_to_device(slot, sizeof(int), 0x0000004000120000);
	}
	else if(PRR_Slot==3){	
    		write_to_device(slot, sizeof(int), 0x0000004000130000);
	}
        else if(PRR_Slot==4){
                write_to_device(slot, sizeof(int), 0x0000004000140000);
        }
        else if(PRR_Slot==5){
                write_to_device(slot, sizeof(int), 0x0000004000150000);
        }
        else if(PRR_Slot==6){
                write_to_device(slot, sizeof(int), 0x0000004000160000);
        }
        else if(PRR_Slot==7){
                write_to_device(slot, sizeof(int), 0x0000004000170000);
        }


	read_from_device(status, sizeof(int), 0x0000004010000110);
	cout << endl;
	cout << endl;
	//cout << "The status of ICAP " << status[0] << endl;
	cout << "Reconfiguring PRR " << PRR_Slot << endl;
	write_to_device(bin_size, sizeof(int), 0x0000004010000108);
	write_bitstream(binFile,in_file.tellg(), 0x0000004014000000);

	status[0]=0;
	int number = 0;

	while (number!=1){
		read_from_device(status, sizeof(int), 0x0000004010000110);
		number = bitExtracted(status[0],1,1);
	}
	slot[0]=0x0;

	if(PRR_Slot==0){
		write_to_device(slot, sizeof(int), 0x0000004000100000);
	}
        else if(PRR_Slot==1){
    		write_to_device(slot, sizeof(int), 0x0000004000110000);
        }
	else if(PRR_Slot==2){	
    		write_to_device(slot, sizeof(int), 0x0000004000120000);
	}
	else if(PRR_Slot==3){	
    		write_to_device(slot, sizeof(int), 0x0000004000130000);
	}
	else if(PRR_Slot==4){
                write_to_device(slot, sizeof(int), 0x0000004000140000);
        }
        else if(PRR_Slot==5){
                write_to_device(slot, sizeof(int), 0x0000004000150000);
        }
        else if(PRR_Slot==6){
                write_to_device(slot, sizeof(int), 0x0000004000160000);
        }
        else if(PRR_Slot==7){
                write_to_device(slot, sizeof(int), 0x0000004000170000);
        }


	cout << endl;
	cout << "Device is programmed successfully" << endl;
}

void Execute_Task(int PRR_Slot){
//Initiate Accelerator
	cout << " Execute Kernel " << PRR_Slot << endl;

	int lite[1]={0x00000001};

	if(PRR_Slot==0){
   		write_to_device(lite,sizeof(int),0x000000000);
   	}
   	else if(PRR_Slot==1){
   		write_to_device(lite,4*sizeof(int),0x40000E000);
   	}		
}

void Finish_Task(int PRR_Slot){
//Initiate Accelerator
	cout << endl;
	cout << endl;
	cout << "Wait until PRR " << PRR_Slot << " is finished " << endl;

  	int end[1]={0x00000000};
  	int bits=0;


	if(PRR_Slot==0){
		while(bits==0){
			read_from_device(end, sizeof(int), 0x0000004000000000);
			bits = bitExtracted(end[0],1,3);
		    cout << "The status of the PRR 0 is " << bits << endl;

		}  
	}
   	else if(PRR_Slot==1){
		while(bits==0){
			read_from_device(end, sizeof(int), 0x0000004000010000);
			bits = bitExtracted(end[0],1,3);
		    cout << "The status of the PRR 1 is " << bits << endl;
		}   	
	}
	else if(PRR_Slot==2){
		while(bits==0){	
			read_from_device(end, sizeof(int), 0x0000004000020000);
			bits = bitExtracted(end[0],1,3);
			cout << "The status of the PRR 2 is " << bits << endl;
		}
	}
	else if(PRR_Slot==3){
		while(bits==0){	
			read_from_device(end, sizeof(int), 0x0000004000030000);
			bits = bitExtracted(end[0],1,3);
			cout << "The status of the PRR 3 is " << bits << endl;
		}
	}
}


void FPGA_write(void* data, uint64_t start_address, uint64_t bytes){
	cout << endl;
	cout<<"Sending data to FPGA address " << start_address << " and size " << bytes << endl;

	write_to_device(data, bytes, start_address);
	
}

void FPGA_read(void* data, uint64_t start_address, uint64_t bytes){
	cout << endl;
	cout << endl;
	cout<<"Receiving data from FPGA address " << start_address << " and size " << bytes << endl;

	read_from_device(data, bytes, start_address /*+0x0000001000000000*/);

}


