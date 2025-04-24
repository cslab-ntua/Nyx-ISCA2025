/*===============================================================*/
/*                                                               */
/*                        rendering.cpp                          */
/*                                                               */
/*                 C++ kernel for 3D Rendering                   */
/*                                                               */
/*===============================================================*/

#include "typedefs.h"

#ifdef NONDATAFLOW

/*======================UTILITY FUNCTIONS========================*/


// Determine whether three vertices of a trianlgLe
// (x0,y0) (x1,y1) (x2,y2) are in clockwise order by Pineda algorithm
// if so, return a number > 0
// else if three points are in line, return a number == 0
// else in counterclockwise order, return a number < 0
int check_clockwise(Triangle_2D triangle_2d){

	int cw;
	cw = (triangle_2d.x2 - triangle_2d.x0) * (triangle_2d.y1 - triangle_2d.y0)
    		   - (triangle_2d.y2 - triangle_2d.y0) * (triangle_2d.x1 - triangle_2d.x0);

	return cw;
}

// swap (x0, y0) (x1, y1) of a Triangle_2D
void clockwise_vertices(Triangle_2D *triangle_2d){

	bit8 tmp_x, tmp_y;
	tmp_x = triangle_2d->x0;
	tmp_y = triangle_2d->y0;

	triangle_2d->x0 = triangle_2d->x1;
	triangle_2d->y0 = triangle_2d->y1;

	triangle_2d->x1 = tmp_x;
	triangle_2d->y1 = tmp_y;
}

// Given a pixel, determine whether it is inside the triangle
// by Pineda algorithm
// if so, return true
// else, return false
bit1 pixel_in_triangle(bit8 x, bit8 y, Triangle_2D triangle_2d){

	int pi0, pi1, pi2;
	pi0 = (x - triangle_2d.x0) * (triangle_2d.y1 - triangle_2d.y0) - (y - triangle_2d.y0) * (triangle_2d.x1 - triangle_2d.x0);
  	pi1 = (x - triangle_2d.x1) * (triangle_2d.y2 - triangle_2d.y1) - (y - triangle_2d.y1) * (triangle_2d.x2 - triangle_2d.x1);
  	pi2 = (x - triangle_2d.x2) * (triangle_2d.y0 - triangle_2d.y2) - (y - triangle_2d.y2) * (triangle_2d.x0 - triangle_2d.x2);

  	return (pi0 >= 0 && pi1 >= 0 && pi2 >= 0);
}

// find the min from 3 integers
bit8 find_min(bit8 in0, bit8 in1, bit8 in2){
	if (in0 < in1){
		if (in0 < in2)
			return in0;
		else
			return in2;
	}
	else{
		if (in1 < in2)
			return in1;
		else
			return in2;
	}
}

// find the max from 3 integers
bit8 find_max(bit8 in0, bit8 in1, bit8 in2){
	if (in0 > in1){
		if (in0 > in2)
			return in0;
		else
			return in2;
	}
	else{
		if (in1 > in2)
			return in1;
		else
			return in2;
	}
}

//read input and project a 3D triangle to a 2D triangle
void projection_rasterization(bit32 *input, CandidatePixel *fragment2, bit16 *size_fragment){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=31920 port=input offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=31920 port=size_fragment offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=1596000 port=fragment2 offset=slave
	#pragma HLS INTERFACE s_axilite port=input bundle=control
	#pragma HLS INTERFACE s_axilite port=size_fragment bundle=control
	#pragma HLS INTERFACE s_axilite port=fragment2 bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control


	//local buffer needed
	Triangle_3D triangle_3d;
	Triangle_2D triangle_2d;
	Triangle_2D triangle_2d_same;
	bit8 max_min[5];
	bit16 max_index;
   	bit8 color = 100;
	bit2 flag;

    TRIANGLES: for (bit16 iter=0; iter<NUM_3D_TRI; iter++){
		#pragma HLS pipeline II=1

        bit32 input_lo  = input[3*iter];
        bit32 input_mi  = input[3*iter+1];
        bit32 input_hi  = input[3*iter+2];

        triangle_3d.x0 = input_lo( 7,  0);
        triangle_3d.y0 = input_lo(15,  8);
        triangle_3d.z0 = input_lo(23, 16);
        triangle_3d.x1 = input_lo(31, 24);
        triangle_3d.y1 = input_mi( 7,  0);
        triangle_3d.z1 = input_mi(15,  8);
        triangle_3d.x2 = input_mi(23, 16);
        triangle_3d.y2 = input_mi(31, 24);
        triangle_3d.z2 = input_hi( 7,  0);

        triangle_2d.x0 = triangle_3d.x0;
        triangle_2d.y0 = triangle_3d.y0;
        triangle_2d.x1 = triangle_3d.x1;
        triangle_2d.y1 = triangle_3d.y1;
        triangle_2d.x2 = triangle_3d.x2;
        triangle_2d.y2 = triangle_3d.y2;
        triangle_2d.z  = triangle_3d.z0 / 3 + triangle_3d.z1 / 3 + triangle_3d.z2 / 3;

        // clockwise the vertices of input 2d triangle
        if(check_clockwise(triangle_2d)==0){
        	flag = 1;
        }
        else{
        	flag = 0;
        }

        if(check_clockwise(triangle_2d)<0)
        	clockwise_vertices(&(triangle_2d));

        // copy the same 2D triangle
        triangle_2d_same.x0 = triangle_2d.x0;
        triangle_2d_same.y0 = triangle_2d.y0;
        triangle_2d_same.x1 = triangle_2d.x1;
        triangle_2d_same.y1 = triangle_2d.y1;
        triangle_2d_same.x2 = triangle_2d.x2;
        triangle_2d_same.y2 = triangle_2d.y2;
        triangle_2d_same.z  = triangle_2d.z ;

        // find the rectangle bounds of 2D triangles
        max_min[0] = find_min( triangle_2d.x0, triangle_2d.x1, triangle_2d.x2 );
        max_min[1] = find_max( triangle_2d.x0, triangle_2d.x1, triangle_2d.x2 );
        max_min[2] = find_min( triangle_2d.y0, triangle_2d.y1, triangle_2d.y2 );
        max_min[3] = find_max( triangle_2d.y0, triangle_2d.y1, triangle_2d.y2 );
        max_min[4] = max_min[1] - max_min[0];

        // calculate index for searching pixels
        max_index = (max_min[1] - max_min[0]) * (max_min[3] - max_min[2]);

       	bit16 j = 0;
        if(flag){
        	size_fragment[iter] = 0;
       	    continue;
        }

       	RAST2: for (bit16 k=0; k<max_index; k++){
       		#pragma HLS PIPELINE II=1
       	    bit8 x = max_min[0] + k%max_min[4];
       	    bit8 y = max_min[2] + k/max_min[4];

       	    CandidatePixel curr_frag;

       	    if(pixel_in_triangle(x,y,triangle_2d_same)){
       	    	curr_frag.x = x;
       	    	curr_frag.y = y;
       	    	curr_frag.z = triangle_2d_same.z;
       	    	curr_frag.color = color;
       	        fragment2[iter*500+j] = curr_frag;
       	        j++;
       	    }
       	}
       	size_fragment[iter] = j;

	}
}

void zculling(CandidatePixel *fragment2, bit16 *size_fragment, Pixel *pixels, bit16 *size_pixels){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=1596000 port=fragment2 offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=31920 port=size_fragment offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=1596000 port=pixels offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=31920 port=size_pixels offset=slave
	#pragma HLS INTERFACE s_axilite port=fragment2 bundle=control
	#pragma HLS INTERFACE s_axilite port=size_fragment bundle=control
	#pragma HLS INTERFACE s_axilite port=pixels bundle=control
	#pragma HLS INTERFACE s_axilite port=size_pixels bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control


	//initilize the z-buffer in rendering first triangle for an image
	static bit8 z_buffer[MAX_X][MAX_Y];

	for (bit16 m = 0; m<NUM_3D_TRI; m++){

		if (m==0){
			ZCULLING_INIT_ROW: for(bit16 i=0; i<MAX_X; i++){
        	#pragma HLS PIPELINE II=1
				ZCULLING_INIT_COL: for(bit16 j=0; j<MAX_Y; j++){
    			z_buffer[i][j] = 255;
				}
			}
		}

		// pixel counter
		bit16 pixel_cntr = 0;
		bit16 size = size_fragment[m];

		// update z-buffer and pixels
		ZCULLING: for (bit16 n=0; n<size; n++){
      		#pragma HLS PIPELINE II=1

			CandidatePixel fragments = fragment2[m*500 + n];
			Pixel curr_pixel;

			if(fragments.z < z_buffer[fragments.y][fragments.x]){
				curr_pixel.x    	= fragments.x;
				curr_pixel.y   		= fragments.y;
				curr_pixel.color 	= fragments.color;
				pixels[m*500 + pixel_cntr] = curr_pixel;
				pixel_cntr++;
				z_buffer[fragments.y][fragments.x] = fragments.z;

			}
		}
       	size_pixels[m] = pixel_cntr;
	}
}

void coloring_outputFB(Pixel *pixels, bit16 *size_pixels, bit32 *output){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=1596000 port=pixels offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=31920 port=size_pixels offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=16384 port=output offset=slave
	#pragma HLS INTERFACE s_axilite port=pixels bundle=control
	#pragma HLS INTERFACE s_axilite port=size_pixels bundle=control
	#pragma HLS INTERFACE s_axilite port=output bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	bit8 frame_buffer[MAX_X][MAX_Y];

	for(bit16 m=0; m<NUM_3D_TRI; m++){
		if(m==0){
			// initilize the framebuffer for a new image
			COLORING_FB_INIT_ROW: for (bit16 i=0; i<MAX_X; i++){
    			#pragma HLS PIPELINE II=1
				COLORING_FB_INIT_COL: for ( bit16 j = 0; j < MAX_Y; j++)
					frame_buffer[i][j] = 0;
			}
		}

		bit16 pixel_size = size_pixels[m];

		//update the framebuffer
		COLORING_FB: for (bit16 i=0; i<pixel_size; i++){
      		#pragma HLS PIPELINE II=1
			Pixel curr_pixel = pixels[m*500 + i];
			frame_buffer[ curr_pixel.x ][ curr_pixel.y ] = curr_pixel.color;
		}
	}

	bit32 out_FB = 0;
	OUTPUT_FB_ROW: for (bit16 i=0; i<MAX_X; i++){
		#pragma HLS PIPELINE II=1
		OUTPUT_FB_COL: for (bit16 j=0; j<MAX_Y; j=j+4){

			out_FB( 7,  0) = frame_buffer[i][j + 0];
			out_FB(15,  8) = frame_buffer[i][j + 1];
			out_FB(23, 16) = frame_buffer[i][j + 2];
			out_FB(31, 24) = frame_buffer[i][j + 3];
			output[i * MAX_Y / 4 + j / 4] = out_FB;
		}
	}
}

#endif
