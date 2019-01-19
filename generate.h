#include <stdio.h>
#include <cuda.h>
#include <time.h>
#include <curand_kernel.h>
#include <curand.h>
#include <time.h>
#include <stdlib.h>

#ifndef GENERATE_H
#define GENERATE_H
#define PATTERN_SIZE 21

//#define N 1500000000

//private char[N] seq1;
//private char[N] seq2;


void check_cuda_error(cudaError_t err){
	if(err != cudaSuccess){
		printf("%s in %s at line %d\n",cudaGetErrorString(err), __FILE__,__LINE__);
		exit(EXIT_FAILURE);	

	}

}


__global__
void printDNA_kernel(char * seq1, char * seq2, int size){
     
    int i= threadIdx.x + blockDim.x*blockIdx.x;
    //int c = 1;
    while(i<size){
        printf("%i. %c - %c\n",i+1, seq1[i],seq2[i]);
        i += blockDim.x+gridDim.x;
    }
    
    
    
}


__global__
void generate_kernel(char * seq1, char * seq2, int n, unsigned int * random){
    
    int r; 
    const char genom[4] = {'T','C','G','A'};
    int i = threadIdx.x + blockDim.x * blockIdx.x;
     while(i<n){
        if(i<n){
        r = random[i] % 4;
        seq1[i] = genom[r];
        switch(seq1[i]){
            case 'A':
                seq2[i] = 'C';
                break;
            case 'C':
                seq2[i] = 'A';
                break;
            case 'T':
                seq2[i] = 'G';
                break;
            case 'G':
                seq2[i] = 'T';
                break;
        }
    }    
        i += blockDim.x+gridDim.x;
    }      
}

__global__
void hide(char * seq1, char * seq2, int r, int mutation_starts){
       char patt[PATTERN_SIZE] ={'A','T','G','C','A','G','G','T','G','A','C','A','T','A','C','C','T','C','A','G','T'} ; // Tay - Sachs disase
       //int s = (mutation_starts % PATTERN_SIZE) - PATTERN_SIZE ;
        //printf("stride: %d\n",s);
        int i= threadIdx.x + blockDim.x*blockIdx.x; 
        int j = (i-1)%PATTERN_SIZE; // if the sequence size is 15000 i+2
        if(r == 0){
            if(i >= mutation_starts && i< (mutation_starts + PATTERN_SIZE)){
            printf("csere: %c-t - pattern %c-re\n", seq1[i], patt[j]);
            seq1[i] = patt[j];
            switch(seq1[i]){
                case 'A':
                    seq2[i] = 'C';
                    //printf("csere: %c-t - %c-re\n", seq1[i], seq2[i]);
                    break;
                case 'C':
                    seq2[i] = 'A';
                    //printf("csere: %c-t - %c-re\n", seq1[i], seq2[i]);
                    break;
                case 'T':
                    seq2[i] = 'G';
                    //printf("csere: %c-t - %c-re\n", seq1[i], seq2[i]);
                    break;
                case 'G':
                    seq2[i] = 'T';
                    //printf("csere: %c-t - %c-re\n", seq1[i], seq2[i]);
                    break;
                }
                //i += blockDim.x+gridDim.x;
            }
        }else{
            if(i >= mutation_starts && i< (mutation_starts + PATTERN_SIZE)){

            seq2[i] = patt[j];
            printf("csere: %c-t - %c-re", seq2[i], patt[i]);
            switch(seq2[i]){
                case 'A':
                    seq1[i] = 'C';
              //      printf("csere: %c-t - %c-re", seq2[i], seq2[i]);
                    break;
                case 'C':
                    seq1[i] = 'A';
                //    printf("csere: %c-t - %c-re", seq2[i], seq2[i]);
                    break;
                case 'T':
                    seq1[i] = 'G';
                  //  printf("csere: %c-t - %c-re", seq2[i], seq2[i]);
                    break;
                case 'G':
                    seq1[i] = 'T';
                    //printf("csere: %c-t - %c-re", seq2[i], seq2[i]);
                    break;
                }
                //i += blockDim.x+gridDim.x;
            }
        
            
        }
        
}
void printDNA(char * seq1, char * seq2, int n){
    int c = 1;
    for(int i=0; i<n; i++){
        printf("%i. %c - %c\n",seq1[i],seq2[i],c);
        c++;
    }
}

void generate(char * h_seq1, char * h_seq2, int n){
//(char * h_seq1, char * h_seq2, char * pattern, int n, int m){
    
    float elapsed_generating = 0;
    float elapsed_hiding = 0;
    float elapsed_printing = 0;
    cudaEvent_t start, stop;
    cudaError_t terr;
    terr = cudaEventCreate(&start);
    check_cuda_error(terr);

    terr = cudaEventCreate(&stop);
    check_cuda_error(terr);

    int size = n * sizeof(char);
    int ms = rand() % n; // mutation starts
    int ws = rand() % 2;  // which sequence
    char *d_seq1;
    char *d_seq2;
    //char * d_pattern;
    printf("the sequence is: %i\n", ws);
    printf("mutation starts at: %i\n", ms+1);
    curandGenerator_t gen;
    unsigned int *devData;
    unsigned int *hostData;

    hostData = (unsigned int*)calloc(n, sizeof(unsigned int));

    cudaMalloc((void**)&devData, n*sizeof(unsigned int));
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_DEFAULT);
    curandSetPseudoRandomGeneratorSeed(gen,1234ULL);
    curandGeneratePoisson(gen, devData, n,n);
    
    cudaMemcpy(hostData, devData, n * sizeof(float), cudaMemcpyDeviceToHost);
    /*
    printf("poisson");
    for(int i = 0; i < n; i++) {
        printf("%i \n", hostData[i]);
    }
    */
   int num_threads;
   if(n>10000000){
        num_threads = 128;
   }else{
       num_threads = 1024;
   }
    cudaError_t err = cudaMalloc((void**) &d_seq1,size);
    check_cuda_error(err);
    cudaMemcpy(d_seq1, h_seq1, size, cudaMemcpyHostToDevice);

    err = cudaMalloc((void**) &d_seq2,size);
    check_cuda_error(err);
    cudaMemcpy(d_seq2, h_seq2, size, cudaMemcpyHostToDevice);

    
    printf("Generating sequences...\n");
    cudaEventRecord(start,0);
    generate_kernel<<<(PATTERN_SIZE/n+num_threads)/PATTERN_SIZE,num_threads>>>(d_seq1, d_seq2, n, devData);
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsed_generating, start, stop);
    printf("The elapsed time for generating sequence on gpu was %.2f ms\n", elapsed_generating);
    
    cudaDeviceSynchronize();
    
    printf("Hiding mutation...\n");

    cudaEventRecord(start,0);
    hide<<<(PATTERN_SIZE/n+num_threads)/PATTERN_SIZE,num_threads>>>(d_seq1,d_seq2, ws, ms);
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsed_hiding, start, stop);
    printf("The elapsed time for hiding sequence on gpu was %.2f ms\n", elapsed_hiding);

    printf("Printing DNA...\n");
    cudaDeviceSynchronize();
    
    cudaEventRecord(start,0);
    printDNA_kernel<<<(PATTERN_SIZE/n+num_threads)/PATTERN_SIZE,num_threads>>>(d_seq1, d_seq2, n); // to see what its generates
    cudaEventRecord(stop,0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsed_printing, start, stop);
    printf("The elapsed time for printing sequence on gpu was %.2f ms\n", elapsed_printing);

    cudaMemcpy(h_seq1, d_seq1, size, cudaMemcpyDeviceToHost);
    cudaMemcpy(h_seq2, d_seq2, size, cudaMemcpyDeviceToHost);
    
   //printDNA(h_seq1, h_seq2, n);
    
    curandDestroyGenerator(gen);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);
    cudaFree(devData);
    cudaFree(d_seq1);
    cudaFree(d_seq2);
    free(hostData);
}





#endif