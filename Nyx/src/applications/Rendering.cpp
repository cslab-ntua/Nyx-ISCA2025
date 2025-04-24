#include "Rendering.h"
#include "input_data.h"

#include <iostream>
#include <fstream>
#include <cmath>

void Rendering_input(){

   	Triangle_4D *input = (Triangle_4D*)malloc(sizeof(Triangle_4D) * NUM_3D_TRI);

    // pack input data for better performance
    for ( int i = 0; i < NUM_3D_TRI; i++){
    	input[i].x0    = triangle_3ds[i].x0;
    	input[i].y0    = triangle_3ds[i].y0;
    	input[i].z0    = triangle_3ds[i].z0;
    	input[i].x1    = triangle_3ds[i].x1;
    	input[i].y1    = triangle_3ds[i].y1;
    	input[i].z1    = triangle_3ds[i].z1;
    	input[i].x2    = triangle_3ds[i].x2;
    	input[i].y2    = triangle_3ds[i].y2;
    	input[i].z2    = triangle_3ds[i].z2;
    	input[i].k0    = 0;
    	input[i].k1    = 0;
    	input[i].k2    = 0;
    }

   	write_fpga(input, sizeof(Triangle_4D) * NUM_3D_TRI, RENDERING_INPUT);
    free(input);

}

void check_results(bit8* output){
    
    // print result
  	std::ofstream ofile;
	const size_t size = 1024;
        char buffer[size];
        getcwd(buffer, size);
        std::string pwd = buffer;
  	ofile.open(pwd + "/applications/outputs.txt");

    if (ofile.is_open()){
    ofile << "Image After Rendering: \n";
    for (int j = MAX_X - 1; j >= 0; j -- ){
      for (int i = 0; i < MAX_Y; i ++ ){
        int pix;
          pix = output[i * MAX_Y + j];
        if (pix)
          ofile << "1";
        else
          ofile << "0";
      }
      ofile << std::endl;
    }
  }
  else{
    std::cout << "Failed to create output file!" << std::endl;
  }
}


void Rendering_output(){

	bit8 *output = (bit8*)malloc(sizeof(bit8) * MAX_X * MAX_Y);
   	read_fpga(output, sizeof(bit8)*MAX_X*MAX_Y, RENDERING_OUTPUT);
   	check_results(output);
    free(output);

}
