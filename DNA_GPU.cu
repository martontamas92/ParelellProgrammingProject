#include <stdio.h>
#include "generate.h"
//#include "CPU_search.h"
#include "GPU_search.cuh"


#define DNA_SEQ_SIZE_MAX 1500000 //tizenöt millió
#define DNA_SEQ_SIZE 1500
#define PATTERN_SIZE 21

int main(){
    // first we generate the DNA sequence
    char seq1[DNA_SEQ_SIZE];
    char seq2[DNA_SEQ_SIZE];
    char patt[PATTERN_SIZE] ={'A','T','G','C','A','G','G','T','G','A','C','A','T','A','C','C','T','C','A','G','T'} ; // Tay - Sachs disase
  

    generate(seq1, seq2, DNA_SEQ_SIZE); //runs fine till 15 million size then segmentation fault
    

    // GPU algorithms
     GPU_KMP(seq1, patt, PATTERN_SIZE, DNA_SEQ_SIZE);
     GPU_KMP(seq2, patt, PATTERN_SIZE, DNA_SEQ_SIZE);
     GPU_RK(seq1, patt, DNA_SEQ_SIZE, PATTERN_SIZE);
     GPU_RK(seq2, patt, DNA_SEQ_SIZE, PATTERN_SIZE);
    
 
    return 0;
}

