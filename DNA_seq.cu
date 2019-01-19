#include <stdio.h>
#include "generate.h"
#include "CPU_search.h"
//#include "GPU_search.cuh"



#define DNA_SEQ_SIZE 1500   // fifteen hundred with this cpu algoritmhs works fast
#define PATTERN_SIZE 21

int main(){
    // first we generate the DNA sequence
    char seq1[DNA_SEQ_SIZE];
    char seq2[DNA_SEQ_SIZE];
    char patt[PATTERN_SIZE] ={'A','T','G','C','A','G','G','T','G','A','C','A','T','A','C','C','T','C','A','G','T'} ; // Tay - Sachs disase
  //  char text[14] = "ABCABAABCABAC";
	//char pattern[4] = "CAB";

    generate(seq1, seq2, DNA_SEQ_SIZE); //runs fine till 15 million size then segmentation fault
    //CPU algorithms
//	KMP(text, pattern, 13, 3); //test run - works fine
//    TestQS(); //test run - works fine 
//    RKTest(); //test run - works fine
  // KMP(seq1, patt, DNA_SEQ_SIZE, PATTERN_SIZE);
  //search(seq1, patt); // QS live
  //RK(patt, seq1, 101);

   
    
 
    return 0;
}

