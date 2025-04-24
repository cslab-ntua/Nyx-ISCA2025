#include "typedefs.h"
#include "ap_int.h"

#ifdef NONDATAFLOW

void padImage(DTYPE *src, DTYPE *dst,  int srcm,  int srcn,  int dstm,  int dstn){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS INTERFACE s_axilite port=dstm bundle=control
	#pragma HLS INTERFACE s_axilite port=dstn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	bool image;

	for(int i=0; i<IMAGE_MAX_M; i++){
		if(i<dstm){
			for(int j=0; j<IMAGE_MAX_N; j++){
				#pragma HLS PIPELINE
				if(j<dstn){
					image = ((i<srcm) && (j<srcn));
					if(image)
						dst[ (i*dstn) + j] = src[ (i*srcn) + j];
					else
						dst[ (i*dstn) + j] = DZERO;
				}
			}
		}
	}
}

/*void extractBlockD(DTYPE *src, DTYPE *dst,  int srcm,  int srcn){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	#pragma HLS inline
	DTYPE buffer0[8];
	DTYPE buffer1[8];
	DTYPE buffer2[8];
	DTYPE buffer3[8];
	DTYPE buffer4[8];
	DTYPE buffer5[8];
	DTYPE buffer6[8];
	DTYPE buffer7[8];
	int count = 0;
	int loc = 0;

	loop1:for(int i=0; i<IMAGE_MAX_M; i+=8)
		if(i<srcm){
			loop2:for(int j=0; j<IMAGE_MAX_N; j+=8)
				if(j<srcn){
					printf("%d\n",i*IMAGE_MAX_N+j);
					loop3:for(int k=0; k<8; k++){
						loc = ((i+k)*srcn + j);
						#pragma HLS PIPELINE
						buffer0[k] = src[((i+0)*srcn + j) + k];
						buffer1[k] = src[((i+1)*srcn + j) + k];
						buffer2[k] = src[((i+2)*srcn + j) + k];
						buffer3[k] = src[((i+3)*srcn + j) + k];
						buffer4[k] = src[((i+4)*srcn + j) + k];
						buffer5[k] = src[((i+5)*srcn + j) + k];
						buffer6[k] = src[((i+6)*srcn + j) + k];
						buffer7[k] = src[((i+7)*srcn + j) + k];
					}

					loop4:for(int k=0; k<8; k++){
						#pragma HLS PIPELINE
						dst[(count) + (8*0) + k] = buffer0[k];
						dst[(count) + (8*1) + k] = buffer1[k];
						dst[(count) + (8*2) + k] = buffer2[k];
						dst[(count) + (8*3) + k] = buffer3[k];
						dst[(count) + (8*4) + k] = buffer4[k];
						dst[(count) + (8*5) + k] = buffer5[k];
						dst[(count) + (8*6) + k] = buffer6[k];
						dst[(count) + (8*7) + k] = buffer7[k];
					}
					count += 64;
				}
		}
}*/

void extractBlockD(DTYPE *src, DTYPE *dst,  int srcm,  int srcn){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	DTYPE buffer[8][8];
	int count = 0;

	loop1:for(int i=0; i<IMAGE_MAX_M; i++)
		if(i<srcm){
			loop2:for(int j=0; j<IMAGE_MAX_N; j++)
				if(j<srcn){
					loop3:for(int k=0; k<8; k++){
						loop5: for(int l=0; l<8; l++){
							#pragma HLS PIPELINE
							buffer[k][l] = src[((i+l)*srcn + j) + k];
						}
					}

					loop4:for(int k=0; k<8; k++){
						loop6: for(int l=0; l<8; l++){
							#pragma HLS PIPELINE
							dst[(count) + (8*l) + k] = buffer[k][l];
						}
					}
					count += 64;
				}
		}
}

void matmul_fast(DTYPE srcA[8][8],DTYPE srcB[8][8], DTYPE dst[8][8]){

	DTYPE tmp = DZERO;
	DTYPE sum0 = DZERO;
	DTYPE sum1 = DZERO;
	DTYPE sum2 = DZERO;
	DTYPE sum3 = DZERO;
	DTYPE sum4 = DZERO;
	DTYPE sum5 = DZERO;
	DTYPE sum6 = DZERO;
	DTYPE sum7 = DZERO;

	for(int i=0; i<8; i++){
		for(int j=0 ; j<8; j++)	{
			#pragma HLS PIPELINE
			sum0 = srcA[i][0] * srcB[0][j];
			sum1 = srcA[i][1] * srcB[1][j];
			sum2 = srcA[i][2] * srcB[2][j];
			sum3 = srcA[i][3] * srcB[3][j];
			sum4 = srcA[i][4] * srcB[4][j];
			sum5 = srcA[i][5] * srcB[5][j];
			sum6 = srcA[i][6] * srcB[6][j];
			sum7 = srcA[i][7] * srcB[7][j];

			tmp = sum0 + sum1 + sum2 + sum3 +
					sum4 + sum5 + sum6 + sum7;

			dst[i][j] = tmp;
		}
	}
}

void dct(DTYPE *src, DTYPE *dst, int srcm, int srcn){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control


	float dctMat[8][8] = {
	{ 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553 },
	{ 0.490393, 0.41574, 0.277797, 0.097565, -0.0975196, -0.277759, -0.415714, -0.490384 },
	{ 0.461942, 0.191358, -0.191315, -0.461924, -0.46196, -0.191401, 0.191272, 0.461907 },
	{ 0.41574, -0.0975196, -0.490384, -0.277836, 0.27772, 0.490411, 0.0976559, -0.415662 },
	{ 0.353562, -0.353529, -0.353594, 0.353496, 0.353627, -0.353463, -0.35366, 0.353431 },
	{ 0.277797, -0.490384, 0.0974742, 0.415791, -0.415662, -0.0977013, 0.490429, -0.277605 },
	{ 0.191358, -0.46196, 0.461907, -0.191229, -0.191486, 0.462013, -0.461853, 0.191101 },
	{ 0.097565, -0.277836, 0.415791, -0.49042, 0.490357, -0.415611, 0.277566, -0.097247 }
	};

	//Transposed
	float dctMatT[8][8] = {
	{ 0.353553, 0.490393, 0.461942, 0.41574, 0.353562, 0.277797, 0.191358, 0.097565 },
	{ 0.353553, 0.41574, 0.191358, -0.0975196, -0.353529, -0.490384, -0.46196, -0.277836 },
	{ 0.353553, 0.277797, -0.191315, -0.490384, -0.353594, 0.0974742, 0.461907, 0.415791 },
	{ 0.353553, 0.097565, -0.461924, -0.277836, 0.353496, 0.415791, -0.191229, -0.49042 },
	{ 0.353553, -0.0975196, -0.46196, 0.27772, 0.353627, -0.415662, -0.191486, 0.490357 },
	{ 0.353553, -0.277759, -0.191401, 0.490411, -0.353463, -0.0977013, 0.462013, -0.415611 },
	{ 0.353553, -0.415714, 0.191272, 0.0976559, -0.35366, 0.490429, -0.461853, 0.277566 },
	{ 0.353553, -0.490384, 0.461907, -0.415662, 0.353431, -0.277605, 0.191101, -0.097247 }
	};

	DTYPE srcBlk[8][8];
	DTYPE tmpBlk[8][8];
	DTYPE dstBlk[8][8];

	#pragma HLS ARRAY_PARTITION variable=dctMat cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=srcBlk cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=tmpBlk cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=tmpBlk cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=dctMatT cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=dstBlk cyclic factor=2 dim=1

	for(int m=0; m<IMAGE_MAX_M*IMAGE_MAX_N; m+=64)
		if(m<(srcm*srcn)){
			for(int i=0; i<8; i++)
				for(int j=0; j<8; j++){
					#pragma HLS PIPELINE
					srcBlk[i][j] = src[m + (i*8) +j];
				}

			matmul_fast(dctMat, srcBlk, tmpBlk);
			matmul_fast(tmpBlk, dctMatT, dstBlk);

			for(int i=0; i<8; i++)
				for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					dst[m + (i*8) +j] = dstBlk[i][j];
				}
		}
}

void idct(DTYPE *src, DTYPE *dst, int srcm, int srcn){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	float dctMat[8][8] = {
	{ 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553, 0.353553 },
	{ 0.490393, 0.41574, 0.277797, 0.097565, -0.0975196, -0.277759, -0.415714, -0.490384 },
	{ 0.461942, 0.191358, -0.191315, -0.461924, -0.46196, -0.191401, 0.191272, 0.461907 },
	{ 0.41574, -0.0975196, -0.490384, -0.277836, 0.27772, 0.490411, 0.0976559, -0.415662 },
	{ 0.353562, -0.353529, -0.353594, 0.353496, 0.353627, -0.353463, -0.35366, 0.353431 },
	{ 0.277797, -0.490384, 0.0974742, 0.415791, -0.415662, -0.0977013, 0.490429, -0.277605 },
	{ 0.191358, -0.46196, 0.461907, -0.191229, -0.191486, 0.462013, -0.461853, 0.191101 },
	{ 0.097565, -0.277836, 0.415791, -0.49042, 0.490357, -0.415611, 0.277566, -0.097247 }
	};

	//Transposed
	float dctMatT[8][8] = {
	{ 0.353553, 0.490393, 0.461942, 0.41574, 0.353562, 0.277797, 0.191358, 0.097565 },
	{ 0.353553, 0.41574, 0.191358, -0.0975196, -0.353529, -0.490384, -0.46196, -0.277836 },
	{ 0.353553, 0.277797, -0.191315, -0.490384, -0.353594, 0.0974742, 0.461907, 0.415791 },
	{ 0.353553, 0.097565, -0.461924, -0.277836, 0.353496, 0.415791, -0.191229, -0.49042 },
	{ 0.353553, -0.0975196, -0.46196, 0.27772, 0.353627, -0.415662, -0.191486, 0.490357 },
	{ 0.353553, -0.277759, -0.191401, 0.490411, -0.353463, -0.0977013, 0.462013, -0.415611 },
	{ 0.353553, -0.415714, 0.191272, 0.0976559, -0.35366, 0.490429, -0.461853, 0.277566 },
	{ 0.353553, -0.490384, 0.461907, -0.415662, 0.353431, -0.277605, 0.191101, -0.097247 }
	};

	DTYPE srcBlk[8][8];
	DTYPE tmpBlk[8][8];
	DTYPE dstBlk[8][8];

	#pragma HLS ARRAY_PARTITION variable=dctMatT cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=srcBlk cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=tmpBlk cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=tmpBlk cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=dctMat cyclic factor=2 dim=1
	#pragma HLS ARRAY_PARTITION variable=dstBlk cyclic factor=2 dim=1

	for(int m=0; m<IMAGE_MAX_M*IMAGE_MAX_N; m+=64)
		if(m<(srcm*srcn)){
			for(int i=0; i<8; i++)
				for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					srcBlk[i][j] = src[m + (i*8) +j];
				}

			matmul_fast(dctMatT, srcBlk, tmpBlk);
			matmul_fast(tmpBlk, dctMat, dstBlk);

			for(int i=0; i<8; i++)
				for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					dst[m + (i*8) +j] = dstBlk[i][j];
				}
		}
}

void quantize(DTYPE *src, QTYPE *dst, int srcm, int srcn){

	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	float stdQuantizationMatrixInv[8][8] = {
	{ 0.063, 0.091, 0.100, 0.063, 0.042, 0.025, 0.020, 0.016 },
	{ 0.083, 0.082, 0.071, 0.053, 0.038, 0.017, 0.017, 0.018 },
	{ 0.071, 0.077, 0.063, 0.042, 0.025, 0.018, 0.014, 0.018 },
	{ 0.071, 0.059, 0.045, 0.034, 0.020, 0.011, 0.013, 0.016 },
	{ 0.056, 0.045, 0.027, 0.018, 0.015, 0.009, 0.010, 0.013 },
	{ 0.042, 0.029, 0.018, 0.016, 0.012, 0.010, 0.009, 0.011 },
	{ 0.020, 0.016, 0.013, 0.011, 0.010, 0.008, 0.008, 0.010 },
	{ 0.014, 0.011, 0.011, 0.010, 0.009, 0.010, 0.010, 0.010 }
	};

	DTYPE srcBlk[8][8];
	QTYPE dstBlk[8][8];

	#pragma HLS ARRAY_PARTITION variable=stdQuantizationMatrixInv cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=srcBlk cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=dstBlk cyclic factor=2 dim=2

	DTYPE tmpD0;
	DTYPE tmpD1;
	DTYPE tmpD2;
	DTYPE tmpD3;
	DTYPE tmpD4;
	DTYPE tmpD5;
	DTYPE tmpD6;
	DTYPE tmpD7;

	QTYPE tmpQ0;
	QTYPE tmpQ1;
	QTYPE tmpQ2;
	QTYPE tmpQ3;
	QTYPE tmpQ4;
	QTYPE tmpQ5;
	QTYPE tmpQ6;
	QTYPE tmpQ7;


	loop1:for(int m=0; m<IMAGE_MAX_M*IMAGE_MAX_N; m+=64)
		if(m<(srcm*srcn)){
			loop2:for(int i=0; i<8; i++)
				loop3:for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					srcBlk[i][j] = src[m + (i*8) +j];
				}

			loop4:for(int i=0; i<8; i++){
			#pragma HLS PIPELINE
				tmpD0 = srcBlk[i][0]*stdQuantizationMatrixInv[i][0];
				tmpD1 = srcBlk[i][1]*stdQuantizationMatrixInv[i][1];
				tmpD2 = srcBlk[i][2]*stdQuantizationMatrixInv[i][2];
				tmpD3 = srcBlk[i][3]*stdQuantizationMatrixInv[i][3];
				tmpD4 = srcBlk[i][4]*stdQuantizationMatrixInv[i][4];
				tmpD5 = srcBlk[i][5]*stdQuantizationMatrixInv[i][5];
				tmpD6 = srcBlk[i][6]*stdQuantizationMatrixInv[i][6];
				tmpD7 = srcBlk[i][7]*stdQuantizationMatrixInv[i][7];

				tmpQ0 = (int)tmpD0;
				tmpQ1 = (int)tmpD1;
				tmpQ2 = (int)tmpD2;
				tmpQ3 = (int)tmpD3;
				tmpQ4 = (int)tmpD4;
				tmpQ5 = (int)tmpD5;
				tmpQ6 = (int)tmpD6;
				tmpQ7 = (int)tmpD7;

				dstBlk[i][0] =tmpQ0;
				dstBlk[i][1] =tmpQ1;
				dstBlk[i][2] =tmpQ2;
				dstBlk[i][3] =tmpQ3;
				dstBlk[i][4] =tmpQ4;
				dstBlk[i][5] =tmpQ5;
				dstBlk[i][6] =tmpQ6;
				dstBlk[i][7] =tmpQ7;
			}

			loop6:for(int i=0; i<8; i++)
				loop7:for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					dst[m + (i*8) +j] = dstBlk[i][j];
				}
		}
}

void dequantize(QTYPE *src, DTYPE *dst, int srcm, int srcn){
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	float stdQuantizationMatrix[8][8] = {
	{16.00f, 11.00f, 10.00f, 16.00f, 24.00f, 40.00f, 51.00f, 61.00f},
	{12.00f, 12.00f, 14.00f, 19.00f, 26.00f, 58.00f, 60.00f, 55.00f},
	{14.00f, 13.00f, 16.00f, 24.00f, 40.00f, 57.00f, 69.00f, 56.00f},
	{14.00f, 17.00f, 22.00f, 29.00f, 51.00f, 87.00f, 80.00f, 62.00f},
	{18.00f, 22.00f, 37.00f, 56.00f, 68.00f, 109.00f, 103.00f, 77.00f},
	{24.00f, 35.00f, 55.00f, 64.00f, 81.00f, 104.00f, 113.00f, 92.00f},
	{49.00f, 64.00f, 78.00f, 87.00f, 103.00f, 121.00f, 120.00f, 101.00f},
	{72.00f, 92.00f, 95.00f, 98.00f, 112.00f, 100.00f, 103.00f, 99.00f}
	};

	QTYPE srcBlk[8][8];
	DTYPE dstBlk[8][8];

	#pragma HLS ARRAY_PARTITION variable=stdQuantizationMatrix cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=srcBlk cyclic factor=2 dim=2
	#pragma HLS ARRAY_PARTITION variable=dstBlk cyclic factor=2 dim=2

	DTYPE tmpD0;
	DTYPE tmpD1;
	DTYPE tmpD2;
	DTYPE tmpD3;
	DTYPE tmpD4;
	DTYPE tmpD5;
	DTYPE tmpD6;
	DTYPE tmpD7;

	QTYPE tmpQ0;
	QTYPE tmpQ1;
	QTYPE tmpQ2;
	QTYPE tmpQ3;
	QTYPE tmpQ4;
	QTYPE tmpQ5;
	QTYPE tmpQ6;
	QTYPE tmpQ7;


	loop1:for(int m=0; m<IMAGE_MAX_M*IMAGE_MAX_N; m+=64)
		if(m<(srcm*srcn)){
			loop2:for(int i=0; i<8; i++)
				loop3:for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					srcBlk[i][j] = src[m + (i*8) +j];
				}

			loop4:for(int i=0; i<8; i++){
			#pragma HLS PIPELINE
				tmpD0 = srcBlk[i][0]*stdQuantizationMatrix[i][0];
				tmpD1 = srcBlk[i][1]*stdQuantizationMatrix[i][1];
				tmpD2 = srcBlk[i][2]*stdQuantizationMatrix[i][2];
				tmpD3 = srcBlk[i][3]*stdQuantizationMatrix[i][3];
				tmpD4 = srcBlk[i][4]*stdQuantizationMatrix[i][4];
				tmpD5 = srcBlk[i][5]*stdQuantizationMatrix[i][5];
				tmpD6 = srcBlk[i][6]*stdQuantizationMatrix[i][6];
				tmpD7 = srcBlk[i][7]*stdQuantizationMatrix[i][7];


				dstBlk[i][0] =tmpD0;
				dstBlk[i][1] =tmpD1;
				dstBlk[i][2] =tmpD2;
				dstBlk[i][3] =tmpD3;
				dstBlk[i][4] =tmpD4;
				dstBlk[i][5] =tmpD5;
				dstBlk[i][6] =tmpD6;
				dstBlk[i][7] =tmpD7;
			}


			loop6:for(int i=0; i<8; i++)
				loop7:for(int j=0; j<8; j++){
				#pragma HLS PIPELINE
					dst[m + (i*8) +j] = dstBlk[i][j];
				}
		}
}

void reconstructBlockD(DTYPE *src, DTYPE *dst,  int srcm,  int srcn){
	#pragma HLS INTERFACE mode=m_axi bundle=gmem0 depth=2073600 port=src offset=slave
	#pragma HLS INTERFACE mode=m_axi bundle=gmem1 depth=2073600 port=dst offset=slave
	#pragma HLS INTERFACE s_axilite port=src bundle=control
	#pragma HLS INTERFACE s_axilite port=dst bundle=control
	#pragma HLS INTERFACE s_axilite port=srcm bundle=control
	#pragma HLS INTERFACE s_axilite port=srcn bundle=control
	#pragma HLS interface s_axilite port=return bundle=control

	DTYPE buffer[8][8];
	int count = 0;
	int loc = 0;

	loop1:for(int i=0; i<IMAGE_MAX_M; i+=8)
		if(i<srcm){
			loop2:for(int j=0; j<IMAGE_MAX_N; j+=8)
				if(j<srcn){
					loop3:for(int k=0; k<8; k++){
						loop5:for (int l=0; l<8; l++){
							#pragma HLS PIPELINE
							buffer[k][l] = src[(count) + (8*l) + k];

						}
					}

					count += 64;

					loop4:for(int k=0; k<8; k++){
						loop6: for(int l=0; l<8; l++){
							#pragma HLS PIPELINE
							dst[((i+l)*srcn + j) + k] = buffer[k][l];
						}
					}
				}
		}
}

#endif
