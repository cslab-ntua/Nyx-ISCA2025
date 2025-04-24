#include "typedefs.h"

// use HLS video library
#include "xf_video_mem.hpp"
#include <stdio.h>

const int max_width = MAX_WIDTH;
const int default_depth = MAX_WIDTH;

#ifdef NONDATAFLOW

void read_preprocess(frames_t* frames, input4_t* frame12_45, input_t* frame3){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=frames offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=frame12_45 offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=frame3 offset=slave
	#pragma HLS INTERFACE s_axilite port=frames bundle=control
	#pragma HLS INTERFACE s_axilite port=frame12_45 bundle=control
	#pragma HLS INTERFACE s_axilite port=frame3 bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	//stream in and organize the inputs
    //static frames_t buf;
    FRAMES_CP_OUTER: for (int r=0; r<MAX_HEIGHT; r++){
    	FRAMES_CP_INNER: for (int c=0; c<MAX_WIDTH; c++){
        #pragma HLS pipeline II=1

            // one wide read
    		frames_t buf = frames[r * MAX_WIDTH + c];

    		input4_t tmp12_45;
    		input_t tmp3;

            // assign values to the FIFOs
    		tmp12_45.val[0] = (input_t)(buf(7 ,  0)) >> 8;
            tmp12_45.val[1] = (input_t)(buf(15,  8)) >> 8;
            tmp12_45.val[2] = (input_t)(buf(31, 24)) >> 8;
            tmp12_45.val[3] = (input_t)(buf(39, 32)) >> 8;
            frame12_45[r * MAX_WIDTH + c] = tmp12_45;

            tmp3 = (input_t)(buf(23, 16)) >> 8;
            frame3[r * MAX_WIDTH + c] = tmp3;

    	}
    }
}

void gradient_xy_calc(input_t* frame, pixel2_t* gradient_xy){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=frame offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=gradient_xy offset=slave
	#pragma HLS INTERFACE s_axilite port=frame bundle=control
	#pragma HLS INTERFACE s_axilite port=gradient_xy bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	// our own line buffer
	static pixel_t buf[5][MAX_WIDTH];
  	#pragma HLS array_partition variable=buf complete dim=1

	// small buffer
	pixel_t smallbuf[5];
  	#pragma HLS array_partition variable=smallbuf complete dim=0

	// window buffer
	xf::cv::Window<5,5,input_t> window;

	const int GRAD_WEIGHTS[] =  {1,-8,0,8,-1};

	GRAD_XY_OUTER: for(int r=0; r<MAX_HEIGHT+2; r++){
		GRAD_XY_INNER: for(int c=0; c<MAX_WIDTH+2; c++){
			#pragma HLS pipeline II=1
			//read out values from current line buffer
			for (int i = 0; i < 4; i ++ )
				smallbuf[i] = buf[i+1][c];
			// the new value is either 0 or read from frame
			if (r<MAX_HEIGHT && c<MAX_WIDTH){
				smallbuf[4] = (pixel_t)(frame[r * MAX_WIDTH + c]);
			}
			else if (c < MAX_WIDTH)
				smallbuf[4] = 0;

			// update line buffer
			if(r<MAX_HEIGHT && c<MAX_WIDTH){

				for (int i = 0; i < 4; i ++ )
					buf[i][c] = smallbuf[i];

				buf[4][c] = smallbuf[4];
			}
			else if(c<MAX_WIDTH){

				for (int i = 0; i < 4; i ++ )
					buf[i][c] = smallbuf[i];

				buf[4][c] = smallbuf[4];
			}

			// manage window buffer
			if(r<MAX_HEIGHT && c<MAX_WIDTH){

				window.shift_pixels_left();

				for (int i = 0; i < 5; i ++ )
					window.insert_pixel(smallbuf[i],i,4);
			}
			else{

				window.shift_pixels_left();
				window.insert_pixel(0,0,4);
				window.insert_pixel(0,1,4);
				window.insert_pixel(0,2,4);
				window.insert_pixel(0,3,4);
				window.insert_pixel(0,4,4);
			}

			// compute gradient
			pixel_t x_grad = 0;
			pixel_t y_grad = 0;
			pixel2_t tmpXY;

			if(r>=4 && r<MAX_HEIGHT && c>=4 && c<MAX_WIDTH){

				GRAD_XY_XYGRAD: for(int i=0; i<5; i++){
					x_grad += window.getval(2,i)*GRAD_WEIGHTS[i];
					y_grad += window.getval(i,2)*GRAD_WEIGHTS[i];
				}

				tmpXY.val[0] = x_grad/12;
				tmpXY.val[1] = y_grad/12;
				gradient_xy[(r-2) * MAX_WIDTH + c-2] = tmpXY;

			}
			else if(r>=2 && c>=2){
				tmpXY.val[0] = 0;
				tmpXY.val[1] = 0;
				gradient_xy[(r-2) * MAX_WIDTH + c-2] = tmpXY;
			}
		}
	}
}

void gradient_z_calc(input4_t* frame12_45, pixel_t* gradient_z){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=frame12_45 offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=gradient_z offset=slave
	#pragma HLS INTERFACE s_axilite port=frame12_45 bundle=control
	#pragma HLS INTERFACE s_axilite port=gradient_z bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	int GRAD_WEIGHTS[] =  {1,-8,0,8,-1};

	GRAD_Z_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		GRAD_Z_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  	#pragma HLS pipeline II=1

		    input4_t tmp12_45 = frame12_45[r * MAX_WIDTH + c];
		    pixel_t tmpz;

		    tmpz = ((pixel_t)( tmp12_45.val[0]*GRAD_WEIGHTS[0]  + tmp12_45.val[1]*GRAD_WEIGHTS[1] + tmp12_45.val[2]*GRAD_WEIGHTS[3] + tmp12_45.val[3]*GRAD_WEIGHTS[4]))/12;
			gradient_z[r * MAX_WIDTH + c] = tmpz;

		}
	}

}

void gradient_weight_y(pixel2_t* gradient_xy, pixel_t* gradient_z, gradient_t* filt_grad){


	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=gradient_xy offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=gradient_z offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=filt_grad offset=slave
	#pragma HLS INTERFACE s_axilite port=gradient_xy bundle=control
	#pragma HLS INTERFACE s_axilite port=gradient_z bundle=control
	#pragma HLS INTERFACE s_axilite port=filt_grad bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::LineBuffer<7,MAX_WIDTH,gradient_t> buf;

	const pixel_t GRAD_FILTER[] = {0.0755, 0.133, 0.1869, 0.2903, 0.1869, 0.133, 0.0755};

	GRAD_WEIGHT_Y_OUTER: for(int r=0; r<MAX_HEIGHT+3; r++){
		GRAD_WEIGHT_Y_INNER: for(int c=0; c<MAX_WIDTH; c++){
		  #pragma HLS pipeline II=1
		  #pragma HLS dependence variable=buf inter false

			if(r<MAX_HEIGHT){
				buf.shift_pixels_up(c);
				gradient_t tmp;
				pixel2_t tmpxy = gradient_xy[r * MAX_WIDTH + c];
				pixel_t tmpz = gradient_z[r * MAX_WIDTH + c];
				tmp.x = tmpxy.val[0];
				tmp.y = tmpxy.val[1];
				tmp.z = tmpz;
				buf.insert_bottom_row(tmp,c);
			}
			else{
				buf.shift_pixels_up(c);
				gradient_t tmp;
				tmp.x = 0;
				tmp.y = 0;
				tmp.z = 0;
				buf.insert_bottom_row(tmp,c);
			}

			gradient_t acc;
			acc.x = 0;
			acc.y = 0;
			acc.z = 0;
			acc.w = 0;

			if(r >= 6 && r<MAX_HEIGHT){
				GRAD_WEIGHT_Y_ACC: for(int i=0; i<7; i++){
					acc.x += buf.getval(i,c).x*GRAD_FILTER[i];
					acc.y += buf.getval(i,c).y*GRAD_FILTER[i];
					acc.z += buf.getval(i,c).z*GRAD_FILTER[i];
				}
				filt_grad[(r-3) * MAX_WIDTH + c] = acc;
			}
			else if(r>=3){
				filt_grad[(r-3) * MAX_WIDTH + c] = acc;
			}
		}
	}
}

void gradient_weight_x(gradient_t* y_filt, gradient_t* filt_grad){


	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=y_filt offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=filt_grad offset=slave
	#pragma HLS INTERFACE s_axilite port=y_filt bundle=control
	#pragma HLS INTERFACE s_axilite port=filt_grad bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::Window<1,7,gradient_t> buf;
	const pixel_t GRAD_FILTER[] = {0.0755, 0.133, 0.1869, 0.2903, 0.1869, 0.133, 0.0755};

	GRAD_WEIGHT_X_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		GRAD_WEIGHT_X_INNER: for(int c=0; c<MAX_WIDTH+3; c++){
      	  #pragma HLS pipeline II=1

			buf.shift_pixels_left();
			gradient_t tmp;

			if(c<MAX_WIDTH){
				tmp = y_filt[r * MAX_WIDTH + c];
			}
			else{
				tmp.x = 0;
				tmp.y = 0;
				tmp.z = 0;
				tmp.w = 0;
			}

			buf.insert_pixel(tmp,0,6);

			gradient_t acc;
			acc.x = 0;
			acc.y = 0;
			acc.z = 0;
			acc.w = 0;

			if(c >= 6 && c<MAX_WIDTH){
				GRAD_WEIGHT_X_ACC: for(int i=0; i<7; i++){
					acc.x += buf.getval(0,i).x*GRAD_FILTER[i];
					acc.y += buf.getval(0,i).y*GRAD_FILTER[i];
					acc.z += buf.getval(0,i).z*GRAD_FILTER[i];
				}
				filt_grad[r * MAX_WIDTH + c-3] = acc;
			}
			else if(c>=3){
				filt_grad[r * MAX_WIDTH + c-3] = acc;
			}
		}
	}
}

void outer_product(gradient_t* gradient, outer_t* out_product){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=gradient offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=out_product offset=slave
	#pragma HLS INTERFACE s_axilite port=gradient bundle=control
	#pragma HLS INTERFACE s_axilite port=out_product bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	OUTER_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		OUTER_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  #pragma HLS pipeline II=1

			gradient_t grad = gradient[r * MAX_WIDTH + c];
			gradient_t tmp_gr;

			outer_pixel_t x = (outer_pixel_t) grad.x;
			outer_pixel_t y = (outer_pixel_t) grad.y;
			outer_pixel_t z = (outer_pixel_t) grad.z;
			outer_t out;
			out.val[0] = (x*x);
			out.val[1] = (y*y);
			out.val[2] = (z*z);
			out.val[3] = (x*y);
			out.val[4] = (x*z);
			out.val[5] = (y*z);
			out.val[6] = 0;
			out.val[7] = 0;
			out_product[r * MAX_WIDTH + c] = out;
		}
	}
}

void tensor_weight_y(outer_t* outer, tensor_t* tensor_y){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=outer offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=tensor_y offset=slave
	#pragma HLS INTERFACE s_axilite port=outer bundle=control
	#pragma HLS INTERFACE s_axilite port=tensor_y bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::LineBuffer<3,MAX_WIDTH,outer_t,RAM_T2P_BRAM,1> buf;
	const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};

	TENSOR_WEIGHT_Y_OUTER: for(int r=0; r<MAX_HEIGHT+1; r++){
		TENSOR_WEIGHT_Y_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  #pragma HLS pipeline II=1

			outer_t tmp;
			buf.shift_pixels_up(c);

			if(r<MAX_HEIGHT){
				tmp = outer[r * MAX_WIDTH + c];
			}
			else{
				TENSOR_WEIGHT_Y_TMP_INIT: for(int i=0; i<6; i++)
					tmp.val[i] = 0;
			}

			buf.insert_bottom_row(tmp,c);

			tensor_t acc;

			TENSOR_WEIGHT_Y_ACC_INIT: for(int k =0; k<6; k++)
				acc.val[k] = 0;

			if (r >= 2 && r < MAX_HEIGHT){
				TENSOR_WEIGHT_Y_TMP_OUTER: for(int i=0; i<3; i++){

					tmp = buf.getval(i,c);
					pixel_t k = TENSOR_FILTER[i];

					TENSOR_WEIGHT_Y_TMP_INNER: for(int component=0; component<6; component++){
						acc.val[component] += tmp.val[component]*k;
					}
				}
			}

			if(r >= 1){
				tensor_y[(r-1) * MAX_WIDTH + c] = acc;
			}
		}
	}
}

void tensor_weight_x(tensor_t* tensor_y, tensor_t* tensor){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=tensor_y offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=tensor offset=slave
	#pragma HLS INTERFACE s_axilite port=tensor_y bundle=control
	#pragma HLS INTERFACE s_axilite port=tensor bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::Window<1,3,tensor_t> buf;
	const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};

	TENSOR_WEIGHT_X_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		TENSOR_WEIGHT_X_INNER: for(int c=0; c<MAX_WIDTH+1; c++){
      	  #pragma HLS pipeline II=1

			buf.shift_pixels_left();
			tensor_t tmp;

			if(c<MAX_WIDTH){
				tmp = tensor_y[r * MAX_WIDTH + c];
			}
			else{
				TENSOR_WEIGHT_X_TMP_INIT: for(int i=0; i<6; i++)
					tmp.val[i] = 0;
			}

			buf.insert_pixel(tmp,0,2);

			tensor_t acc;

			TENSOR_WEIGHT_X_ACC_INIT: for(int k =0; k<6; k++)
				acc.val[k] = 0;

			if (c >= 2 && c < MAX_WIDTH){
				TENSOR_WEIGHT_X_TMP_OUTER: for(int i=0; i<3; i++){
					tmp = buf.getval(0,i);

					TENSOR_WEIGHT_X_TMP_INNER: for(int component=0; component<6; component++){
						acc.val[component] += tmp.val[component]*TENSOR_FILTER[i];
					}
				}
			}

			if(c>=1){
				tensor[r * MAX_WIDTH + c-1] = acc;
			}
		}
	}
}

// compute output flow
//void flow_calc(STREAM256 &tensors, hls::burst_maxi<velocity_t> outputs){
void flow_calc(tensor_t* tensors, velocity_t* outputs){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=446464 port=tensors offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=446464 port=outputs offset=slave
	#pragma HLS INTERFACE s_axilite port=tensors bundle=control
	#pragma HLS INTERFACE s_axilite port=outputs bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	static outer_pixel_t buf[2];

	FLOW_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		FLOW_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  #pragma HLS pipeline II=1

			tensor_t tmp_tensor = tensors[r * MAX_WIDTH + c];

			if(r>=2 && r<MAX_HEIGHT-2 && c>=2 && c<MAX_WIDTH-2){

				calc_pixel_t t1 = (calc_pixel_t) tmp_tensor.val[0];
				calc_pixel_t t2 = (calc_pixel_t) tmp_tensor.val[1];
				calc_pixel_t t3 = (calc_pixel_t) tmp_tensor.val[2];
				calc_pixel_t t4 = (calc_pixel_t) tmp_tensor.val[3];
				calc_pixel_t t5 = (calc_pixel_t) tmp_tensor.val[4];
				calc_pixel_t t6 = (calc_pixel_t) tmp_tensor.val[5];
				calc_pixel_t denom = t1*t2-t4*t4;
				calc_pixel_t numer0 = t6*t4-t5*t2;
				calc_pixel_t numer1 = t5*t4-t6*t1;

				if(denom != 0){
					buf[0] = numer0 / denom;
					buf[1] = numer1 / denom;
				}
				else{
					buf[0] = 0;
					buf[1] = 0;
				}
			}
			else{
				buf[0] = buf[1] = 0;
			}

			velocity_t output;
			output.x = (vel_pixel_t)buf[0];
			output.y = (vel_pixel_t)buf[1];

			outputs[r * MAX_WIDTH + c] = output;
		}
	}
}

#endif
