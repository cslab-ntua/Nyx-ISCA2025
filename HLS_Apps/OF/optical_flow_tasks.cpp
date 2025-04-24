#include "typedefs.h"

// use HLS video library
#include "xf_video_mem.hpp"
#include <stdio.h>

const int max_width = MAX_WIDTH;
const int default_depth = MAX_WIDTH;

#ifdef DATAFLOW

//void read_preprocess(hls::burst_maxi<frames_t> frames, STREAM256 &frame12_45, STREAM256 &frame3){
void read_preprocess(frames_t* frames, STREAM256 &frame12_45, STREAM256 &frame3){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem depth=446464 port=frames offset=slave
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=frame12_45 register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=frame3 register
	#pragma HLS INTERFACE s_axilite port=frames bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

    bit256 tmp32, tmp128;

	//frames.read_request(0, MAX_HEIGHT * MAX_WIDTH);

	//stream in and organize the inputs
    static frames_t buf;
    FRAMES_CP_OUTER: for (int r=0; r<MAX_HEIGHT; r++){
    	FRAMES_CP_INNER: for (int c=0; c<MAX_WIDTH; c++){
        #pragma HLS pipeline II=1

            // one wide read
            //buf = frames.read();//[r * MAX_WIDTH + c];
    		buf = frames[r * MAX_WIDTH + c];

            // assign values to the FIFOs
            input_t tmp1_ = (input_t)(buf(7 ,  0)) >> 8;
            tmp128(31,0) = tmp1_(31,0);

            input_t tmp2_ = (input_t)(buf(15,  8)) >> 8;
            tmp128(63,32) = tmp2_(31,0);

            input_t tmp4_ = (input_t)(buf(31, 24)) >> 8;
            tmp128(95,64) = tmp4_(31,0);

            input_t tmp5_ = (input_t)(buf(39, 32)) >> 8;
            tmp128(127,96) = tmp5_(31,0);

            frame12_45.write(tmp128);

            input_t tmp3_ = (input_t)(buf(23, 16)) >> 8;
            tmp32(31,0) = tmp3_(31,0);

            frame3.write(tmp32);

    	}
    }
}

void gradient_xy_calc(STREAM256 &frame, STREAM256 &gradient_xy){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=frame register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=gradient_xy register
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
				bit256 tmp = frame.read();
				input_t tmp_;
				tmp_(31,0) = tmp(31,0);
				smallbuf[4] = (pixel_t)tmp_;
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
			bit256 tmpXY;

			if(r>=4 && r<MAX_HEIGHT && c>=4 && c<MAX_WIDTH){

				GRAD_XY_XYGRAD: for(int i=0; i<5; i++){
					x_grad += window.getval(2,i)*GRAD_WEIGHTS[i];
					y_grad += window.getval(i,2)*GRAD_WEIGHTS[i];
				}
				x_grad = x_grad/12;
				y_grad = y_grad/12;

				tmpXY(31,0) = x_grad(31,0);
				tmpXY(63,32) = y_grad(31,0);

				gradient_xy.write(tmpXY);
			}
			else if(r>=2 && c>=2){
				tmpXY = 0;
				gradient_xy.write(tmpXY);
			}
		}
	}
}

void gradient_z_calc(STREAM256 &frame12_45, STREAM256 &gradient_z){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=frame12_45 register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=gradient_z register
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	int GRAD_WEIGHTS[] =  {1,-8,0,8,-1};

    bit256 tmpz;

	GRAD_Z_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		GRAD_Z_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  	#pragma HLS pipeline II=1
		    bit256 tmp_ = frame12_45.read();
		    input_t tmp1,tmp2,tmp4,tmp5;
		    pixel_t tmpz_;

		    tmp1(31,0) = tmp_(31,0);
		    tmp2(31,0) = tmp_(63,32);
		    tmp4(31,0) = tmp_(95,64);
		    tmp5(31,0) = tmp_(127,96);
		    tmpz_ = ((pixel_t)( tmp1*GRAD_WEIGHTS[0]  + tmp2*GRAD_WEIGHTS[1] + tmp4*GRAD_WEIGHTS[3] + tmp5*GRAD_WEIGHTS[4]))/12;

			tmpz(31,0) = tmpz_(31,0);

			gradient_z.write(tmpz);

		}
	}

}

void gradient_weight_y(STREAM256 &gradient_xy, STREAM256 &gradient_z, STREAM256 &filt_grad){


	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=gradient_xy register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=gradient_z register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=filt_grad register
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
				bit256 tmpxy = gradient_xy.read();
				bit256 tmpz = gradient_z.read();
				tmp.x(31,0) = tmpxy(31,0);
				tmp.y(31,0) = tmpxy(63,32);
				tmp.z(31,0) = tmpz(31,0);
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

			bit256 filt_tmp;
			if(r >= 6 && r<MAX_HEIGHT){
				GRAD_WEIGHT_Y_ACC: for(int i=0; i<7; i++){

					acc.x += buf.getval(i,c).x*GRAD_FILTER[i];
					acc.y += buf.getval(i,c).y*GRAD_FILTER[i];
					acc.z += buf.getval(i,c).z*GRAD_FILTER[i];
				}
				filt_tmp(31,0) = acc.x(31,0);
				filt_tmp(63,32) = acc.y(31,0);
				filt_tmp(95,64) = acc.z(31,0);
				filt_tmp(127,96) = acc.w(31,0);
	            filt_grad.write(filt_tmp);

			}
			else if(r>=3){
				filt_tmp(31,0) = acc.x(31,0);
				filt_tmp(63,32) = acc.y(31,0);
				filt_tmp(95,64) = acc.z(31,0);
				filt_tmp(127,96) = acc.w(31,0);
	            filt_grad.write(filt_tmp);
			}
		}
	}
}

void gradient_weight_x(STREAM256 &y_filt, STREAM256 &filt_grad){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=y_filt register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=filt_grad register

	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::Window<1,7,gradient_t> buf;
	const pixel_t GRAD_FILTER[] = {0.0755, 0.133, 0.1869, 0.2903, 0.1869, 0.133, 0.0755};

	GRAD_WEIGHT_X_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		GRAD_WEIGHT_X_INNER: for(int c=0; c<MAX_WIDTH+3; c++){
      	  #pragma HLS pipeline II=1

			buf.shift_pixels_left();
			gradient_t tmp;

			if(c<MAX_WIDTH){
				bit256 y_filt_temp = y_filt.read();
				tmp.x(31, 0) = y_filt_temp(31, 0);
				tmp.y(31, 0) = y_filt_temp(63, 32);
				tmp.z(31, 0) = y_filt_temp(95, 64);
				tmp.w(31, 0) = y_filt_temp(127, 96);
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

			bit256 filt_tmp;

			if(c >= 6 && c<MAX_WIDTH){
				GRAD_WEIGHT_X_ACC: for(int i=0; i<7; i++){
					acc.x += buf.getval(0,i).x*GRAD_FILTER[i];
					acc.y += buf.getval(0,i).y*GRAD_FILTER[i];
					acc.z += buf.getval(0,i).z*GRAD_FILTER[i];
				}

				filt_tmp(31,0) = acc.x(31, 0);
				filt_tmp(63,32) = acc.y(31, 0);
				filt_tmp(95,64) = acc.z(31, 0);
				filt_tmp(127,96) = acc.w(31, 0);
	            filt_grad.write(filt_tmp);
			}
			else if(c>=3){
				filt_tmp(31,0) = acc.x(31, 0);
				filt_tmp(63,32) = acc.y(31, 0);
				filt_tmp(95,64) = acc.z(31, 0);
				filt_tmp(127,96) = acc.w(31, 0);
	            filt_grad.write(filt_tmp);
			}
		}
	}
}

void outer_product(STREAM256 &gradient, STREAM256 &out_product){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=gradient register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=out_product register
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	OUTER_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		OUTER_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  #pragma HLS pipeline II=1

			bit256 grad = gradient.read();
			gradient_t tmp_gr;
			tmp_gr.x(31,0) = grad(31,0);
			tmp_gr.y(31,0) = grad(63,32);
			tmp_gr.z(31,0) = grad(95,64);
			tmp_gr.w(31,0) = grad(127,96);

			outer_pixel_t x = (outer_pixel_t) tmp_gr.x;
			outer_pixel_t y = (outer_pixel_t) tmp_gr.y;
			outer_pixel_t z = (outer_pixel_t) tmp_gr.z;
			outer_t out;
			out.val[0] = (x*x);
			out.val[1] = (y*y);
			out.val[2] = (z*z);
			out.val[3] = (x*y);
			out.val[4] = (x*z);
			out.val[5] = (y*z);
			out.val[6] = 0;
			out.val[7] = 0;

			bit256 prod_tmp;
			prod_tmp(31, 0) = out.val[0](31,0);
			prod_tmp(63, 32) = out.val[1](31,0);
			prod_tmp(95, 64) = out.val[2](31,0);
			prod_tmp(127, 96) = out.val[3](31,0);
			prod_tmp(159, 128) = out.val[4](31,0);
			prod_tmp(191, 160) = out.val[5](31,0);
			prod_tmp(223, 192) = out.val[6](31,0);
			prod_tmp(255, 224) = out.val[7](31,0);

			out_product.write(prod_tmp);
		}
	}

}

void tensor_weight_y(STREAM256 &outer, STREAM256 &tensor_y){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=outer register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=tensor_y register
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::LineBuffer<3,MAX_WIDTH,outer_t,RAM_T2P_BRAM,1> buf;
	const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};

	TENSOR_WEIGHT_Y_OUTER: for(int r=0; r<MAX_HEIGHT+1; r++){
		TENSOR_WEIGHT_Y_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  #pragma HLS pipeline II=1

			outer_t tmp;
			buf.shift_pixels_up(c);

			if(r<MAX_HEIGHT){
				bit256 out_tmp = outer.read();
				tmp.val[0](31,0) = out_tmp(31, 0);
				tmp.val[1](31,0) = out_tmp(63, 32);
				tmp.val[2](31,0) = out_tmp(95, 64);
				tmp.val[3](31,0) = out_tmp(127, 96);
				tmp.val[4](31,0) = out_tmp(159, 128);
				tmp.val[5](31,0) = out_tmp(191, 160);
				tmp.val[6](31,0) = out_tmp(223, 192);
				tmp.val[7](31,0) = out_tmp(255, 224);
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
				bit256 tensor_tmp;
				tensor_tmp(31, 0) = acc.val[0](31,0);
				tensor_tmp(63, 32) = acc.val[1](31,0);
				tensor_tmp(95, 64) = acc.val[2](31,0);
				tensor_tmp(127, 96) = acc.val[3](31,0);
				tensor_tmp(159, 128) = acc.val[4](31,0);
				tensor_tmp(191, 160) = acc.val[5](31,0);
				tensor_tmp(223, 192) = acc.val[6](31,0);
				tensor_tmp(255, 224) = acc.val[7](31,0);
				tensor_y.write(tensor_tmp);
			}
		}
	}
}

void tensor_weight_x(STREAM256 &tensor_y, STREAM256 &tensor){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=tensor_y register
	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=tensor register
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	xf::cv::Window<1,3,tensor_t> buf;
	const pixel_t TENSOR_FILTER[] = {0.3243, 0.3513, 0.3243};

	TENSOR_WEIGHT_X_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		TENSOR_WEIGHT_X_INNER: for(int c=0; c<MAX_WIDTH+1; c++){
      	  #pragma HLS pipeline II=1

			buf.shift_pixels_left();
			tensor_t tmp;

			if(c<MAX_WIDTH){
				bit256 tensor_y_tmp = tensor_y.read();
				tmp.val[0](31, 0) = tensor_y_tmp(31, 0);
				tmp.val[1](31, 0) = tensor_y_tmp(63, 32);
				tmp.val[2](31, 0) = tensor_y_tmp(95, 64);
				tmp.val[3](31, 0) = tensor_y_tmp(127, 96);
				tmp.val[4](31, 0) = tensor_y_tmp(159, 128);
				tmp.val[5](31, 0) = tensor_y_tmp(191, 160);
				tmp.val[6](31, 0) = tensor_y_tmp(223, 192);
				tmp.val[7](31, 0) = tensor_y_tmp(255, 224);
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
				bit256 tensor_x_tmp;
				tensor_x_tmp(31, 0) = acc.val[0](31, 0);
				tensor_x_tmp(63, 32) = acc.val[1](31, 0);
				tensor_x_tmp(95, 64) = acc.val[2](31, 0);
				tensor_x_tmp(127, 96) = acc.val[3](31, 0);
				tensor_x_tmp(159, 128) = acc.val[4](31, 0);
				tensor_x_tmp(191, 160) = acc.val[5](31, 0);
				tensor_x_tmp(223, 192) = acc.val[6](31, 0);
				tensor_x_tmp(255, 224) = acc.val[7](31, 0);

				tensor.write(tensor_x_tmp);
			}
		}
	}
}

// compute output flow
//void flow_calc(STREAM256 &tensors, hls::burst_maxi<velocity_t> outputs){
void flow_calc(STREAM256 &tensors, velocity_t* outputs){

	#pragma HLS INTERFACE mode=axis register_mode=both depth=446464 port=tensors register
	#pragma HLS INTERFACE mode=m_axi bundle=gmem depth=446464 port=outputs offset=slave
	#pragma HLS INTERFACE s_axilite port=outputs bundle=control
	#pragma HLS INTERFACE s_axilite port=return bundle=control

	static outer_pixel_t buf[2];

	//outputs.write_request(0, MAX_HEIGHT*MAX_WIDTH);

	FLOW_OUTER: for(int r=0; r<MAX_HEIGHT; r++){
		FLOW_INNER: for(int c=0; c<MAX_WIDTH; c++){
      	  #pragma HLS pipeline II=1

			//tensor_t tmp_tensor = tensors[r][c];
			bit256 tmp_tensor = tensors.read();

			if(r>=2 && r<MAX_HEIGHT-2 && c>=2 && c<MAX_WIDTH-2){
				tensor_t temp;
				temp.val[0](31 ,0) = tmp_tensor(31, 0);
				temp.val[1](31 ,0) = tmp_tensor(63, 32);
				temp.val[2](31 ,0) = tmp_tensor(95, 64);
				temp.val[3](31 ,0) = tmp_tensor(127, 96);
				temp.val[4](31 ,0) = tmp_tensor(159, 128);
				temp.val[5](31 ,0) = tmp_tensor(191, 160);

				calc_pixel_t t1 = (calc_pixel_t) temp.val[0];
				calc_pixel_t t2 = (calc_pixel_t) temp.val[1];
				calc_pixel_t t3 = (calc_pixel_t) temp.val[2];
				calc_pixel_t t4 = (calc_pixel_t) temp.val[3];
				calc_pixel_t t5 = (calc_pixel_t) temp.val[4];
				calc_pixel_t t6 = (calc_pixel_t) temp.val[5];
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

	//outputs.write_response();
}

#endif
