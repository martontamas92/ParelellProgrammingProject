#include<stdio.h>
#include <cuda.h>
#include <time.h>
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include <curand.h>
#include <math.h>

#ifndef GPU_SEARCH_H
#define GPU_SEARCH_H

// KMP algorithm
void preKMP(char * patt, int f[], int m){
	int k;
	f[0] = -1;
	for(int i =0; i<m; i++){
		k = f[i - 1];
        while(k >= 0){
            if(patt[k] == patt[i - 1]){
                break;
            }else{
                k = f[k];
            }
        }
        f[i] = k+1;
	}
}

__global__
void KMP_kernel(char * seq, char * patt, int * f, int * c, int n, int m)
{
	int index = threadIdx.x + blockDim.x * blockIdx.x;
	int i = n * index;
	int j = n * (index + 2) - 1;
	if(i > m){
		return;
	}
	if(j > m){
		j = m;
	}
	int k = 0;
	while(i<k){
		if(k == -1){
			i++;
			k = 0;
		}else if(seq[i] == patt[i]){
			i++;
			k++;
			if(k == n){
				c[i-n] = i-n;
				i = i - k + 1;
			}
		}else{
			k = f[k];
		}
	}
	return;
}

void GPU_KMP(char * seq, char * patt, int m, int n){
    int size = 4;
    int f[m];
    int c[m];
    preKMP(patt, f, m);
    int *d_f;
    int *d_c;
    char * d_seq;
    char * d_patt;

   // time_t rawtime1;
   // time ( &rawtime1 );


    cudaError_t err = cudaMalloc((void**)&d_patt, size*m);
    err = cudaMalloc((void**)&d_seq, size*n);
    err = cudaMalloc((void**)&d_f, size*m);
    err = cudaMalloc((void**)&d_c, size*m);

    cudaMemcpy(d_seq, seq, n*size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_patt, patt, m*size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_f, f, m*size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_c, c, m*size, cudaMemcpyHostToDevice);
   // time_t rawtime2;
   // time ( &rawtime2 );

    float time_elapsed = 0;
    cudaEvent_t start;
	cudaEvent_t stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    cudaEventRecord(start,0);
    KMP_kernel<<<(m/n+1024)/1024,1024>>>(d_seq, d_patt, d_f, d_c, n,m);
    cudaEventRecord(stop,0);

    cudaEventSynchronize(start);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&time_elapsed,start,stop);

    cudaMemcpy(c, d_c, m*4, cudaMemcpyDeviceToHost);
    cudaFree(d_patt);
    cudaFree(d_seq);
    cudaFree(d_f);
    cudaFree(d_c);
       
}

// end of kmp algorithm

// RK algorithm cuda
__device__
int RK_compute_value(char * seq, int length, int d, int q){
    int i = 0;
	int p0 = 0;

	for (i = 0; i < length; ++i) {
		p0 = (d * p0 + seq[i]) % q; 
	}

	return p0;
}
__device__
int rk_matcher(char *str, char *pattern, int d, int q, int n, int m)
{
	int i = 0, j = 0;

	int str_length = n;
	int pattern_length = m;
	int p0 = 0;
	int ts[str_length];

	/* This code block prints what is inside the matrix
	   for (i=0;i<num_cores;i++)
	   {
	   for (j=0;j<el_chunk_len;j++)
	   if (tss[i][j]==0)
	   printf("%c", '0');
	   else
	   printf("%c", tss[i][j]);
	   printf("\n");
	   }
	 */

	//hash value of the pattern
	p0 = RK_compute_value(pattern, pattern_length, d, q);

	//hash value of the first char
	ts[0] = RK_compute_value(str, pattern_length, d, q);

	//p does not change, calculate once
	int p = pow(d,(pattern_length - 1));
	for (i = 1; i < str_length - pattern_length + 1; i++) {
		ts[i] = ((str[i + pattern_length - 1])) * p0 + (ts[i-1] - (str[i-1])/d)%q;
	
	}


	for (i = 0; i <= str_length - pattern_length + 1; ++i) {
		if (ts[i] == p0) {
			for (j = 0; j < pattern_length; ++j) {
				if (pattern[j] != str[i + j]) {
					break;
				} else if (j == pattern_length - 1) {
					printf("%d\n", i);
				}
			}
		}
	}

	return 0;
}
__global__ 
void findHashes(char *d_css, int d_len, int *d_iss, int pattern_length, int d, int p)
{
	int i = 0;
	int ind = d_len * threadIdx.x;
	d_iss += ind;
	d_css += ind;
	d_iss[0] = 0;

	int pw = 1;
	for (; i < pattern_length; i++) {
		d_iss[0] += pw * (d_css[i]);
		pw *= d;
	}
	

	for (i = 1; i < d_len - pattern_length + 1; i++) {
		d_iss[i] = ((d_css[i + pattern_length - 1]) * p + (d_iss[i - 1] - (d_css[i - 1])) / d); 
	}

}

__global__ 
void seekPattern(char *d_css, int d_len, int *d_iss, int pattern_length, char* pattern, int d, int p0) 
{
	int i = 0;
    int j=0;
	int ind = d_len * threadIdx.x;
	d_iss += ind;
	d_css += ind;

	for (i = 0; i < d_len - pattern_length + 1; i++) {
		if (d_iss[i] == p0) {
			for (j = 0; j < pattern_length; j++) {
				if (pattern[j] != d_css[i + j]) {
					break;
				} else if (j == pattern_length - 1) {

			
					printf("pos:%d\n", threadIdx.x*(d_len-pattern_length+1)+i-pattern_length+1);
				}
			}
		}
	}

}

void GPU_RK(char * seq, char * pattern, int n, int m){
	//int d = m;
	int num_cores = 8;
	int size = (int)ceil((float)n/num_cores);
	int padding = size * num_cores - n;
	int el_size = size + m -1;

	char char_matrix[num_cores][el_size];
	int int_matrix[num_cores][el_size];

	char * d_char_matrix;
	char * d_pattern;
	int	 * d_int_matrix; 
	int nchars = num_cores * el_size;
	cudaMalloc((char**)&d_char_matrix, nchars*sizeof(char));
	cudaMalloc((int**)&d_int_matrix, nchars*sizeof(int));
	cudaMalloc((char**)&d_pattern, nchars*sizeof(char));

	//initial zeroes
	for (int i = 0; i < m - 1; i++)
		char_matrix[0][i] = 0;

	//first n-1 cores' characters
	for (int i = 0; i < num_cores - 1; i++)
		for (int j = 0; j < size; j++)
			char_matrix[i][j + m - 1] = seq[i * size + j];

	//last core's characters
	for (int i = (num_cores - 1) * size; i < n; i++){
		int j = 0;
		char_matrix[num_cores - 1][j + m - 1] = seq[i];
		j++;
	}
		

	//last n-1 cores' padding characters
	for (int i = 1; i < num_cores; i++)
		for (int j = 0; j < m - 1; j++)
			char_matrix[i][j] = char_matrix[i - 1][j + size];

	//last core's last paddings
	for (int i = 0; i < padding; i++)
		char_matrix[num_cores - 1][el_size - i - 1] = 0;

	//transfer css to device
	cudaMemcpy(d_char_matrix, char_matrix, nchars, cudaMemcpyHostToDevice);
	cudaMemcpy(d_int_matrix, int_matrix, nchars, cudaMemcpyHostToDevice);
	cudaMemcpy(d_pattern, pattern, m, cudaMemcpyHostToDevice);

	dim3 block(num_cores);
	int p = pow(m, m-1);
	findHashes<<< 1, num_cores >>>(d_char_matrix, el_size, d_int_matrix,m, m, p);
	 //find the hash of the pattern
    int pw = 1;
    int p0=0;
    for (int i=0; i < m; i++) {
        p0 += pw * (pattern[i]);
        pw *= m;
    }
	seekPattern<<<1, num_cores>>>(d_char_matrix, el_size, d_int_matrix, m, d_pattern, m, p0); 
	cudaFree(d_int_matrix);
	cudaFree(d_char_matrix);
	cudaFree(d_pattern);
}


#endif // GPU_SEARCH_H
