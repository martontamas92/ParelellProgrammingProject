#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef CPU_SEARCH_H
#define CPU_SEARCH_H
#define d 256


// Function to implement KMP algorithm
__host__
void KMP(const char* X, const char* Y, int m, int n)
{
	// Base Case 1: Y is NULL or empty
	if (*Y == '\0' || n == 0)
		printf("Pattern occurs with shift 0");

	// Base Case 2: X is NULL or X's length is less than that of Y's
	if (*X == '\0' || n > m)
		printf("Pattern not found");

	// next[i] stores the index of next best partial match
	int next[n + 1];

	for (int i = 0; i < n + 1; i++)
		next[i] = 0;

	for (int i = 1; i < n; i++)
	{
		int j = next[i + 1];

		while (j > 0 && Y[j] != Y[i])
			j = next[j];

		if (j > 0 || Y[j] == Y[i])
			next[i + 1] = j + 1;
	}

	for (int i = 0, j = 0; i < m; i++)
	{
		if (*(X + i) == *(Y + j))
		{
			if (++j == n)
				printf("Pattern occurs with shift %d\n", i - j + 1);
		}
		else if (j > 0) {
			j = next[j];
			i--;	// since i will be incremented in next iteration
		}
	}
}


// preprocessing for strong good suffix rule 
__host__
void preprocess_strong_suffix(int *shift, int *bpos, char *pat, int m) 
{ 
	// m is the length of pattern 
	int i=m, j=m+1; 
	bpos[i]=j; 

	while(i>0) 
	{ 
		/*if character at position i-1 is not equivalent to 
		character at j-1, then continue searching to right 
		of the pattern for border */
		while(j<=m && pat[i-1] != pat[j-1]) 
		{ 
			/* the character preceding the occurence of t in 
			pattern P is different than mismatching character in P, 
			we stop skipping the occurences and shift the pattern 
			from i to j */
			if (shift[j]==0) 
				shift[j] = j-i; 

			//Update the position of next border 
			j = bpos[j]; 
		} 
		/* p[i-1] matched with p[j-1], border is found. 
		store the beginning position of border */
		i--;j--; 
		bpos[i] = j; 
	} 
} 

//Preprocessing for case 2 
__host__
void preprocess_case2(int *shift, int *bpos, char *pat, int m) 
{ 
	int i, j; 
	j = bpos[0]; 
	for(i=0; i<=m; i++) 
	{ 
		/* set the border postion of first character of pattern 
		to all indices in array shift having shift[i] = 0 */
		if(shift[i]==0) 
			shift[i] = j; 

		/* suffix become shorter than bpos[0], use the position of 
		next widest border as value of j */
		if (i==j) 
			j = bpos[j]; 
	} 
} 

/*Search for a pattern in given text using 
Boyer Moore algorithm with Good suffix rule */
__host__
void search(char *text, char *pat) 
{ 
	// s is shift of the pattern with respect to text 
	int s=0, j; 
	int m = strlen(pat); 
	int n = strlen(text); 

	int bpos[m+1], shift[m+1]; 

	//initialize all occurence of shift to 0 
	for(int i=0;i<m+1;i++) shift[i]=0; 

	//do preprocessing 
	preprocess_strong_suffix(shift, bpos, pat, m); 
	preprocess_case2(shift, bpos, pat, m); 

	while(s <= n-m) 
	{ 

		j = m-1; 

		/* Keep reducing index j of pattern while characters of 
			pattern and text are matching at this shift s*/
		while(j >= 0 && pat[j] == text[s+j]) 
			j--; 

		/* If the pattern is present at current shift, then index j 
			will become -1 after the above loop */
		if (j<0) 
		{ 
			printf("pattern occurs at shift = %d\n", s); 
			s += shift[0]; 
		} 
		else
			/*pat[i] != pat[s+j] so shift the pattern 
			shift[j+1] times */
			s += shift[j+1]; 
	} 

}
__host__
void RK(char pat[], char txt[], int q) 
{ 
    int M = strlen(pat); 
    int N = strlen(txt); 
    int i, j; 
    int p = 0; // hash value for pattern 
    int t = 0; // hash value for txt 
    int h = 1; 
  
    // The value of h would be "pow(d, M-1)%q" 
    for (i = 0; i < M-1; i++) 
        h = (h*d)%q; 
  
    // Calculate the hash value of pattern and first 
    // window of text 
    for (i = 0; i < M; i++) 
    { 
        p = (d*p + pat[i])%q; 
        t = (d*t + txt[i])%q; 
    } 
  
    // Slide the pattern over text one by one 
    for (i = 0; i <= N - M; i++) 
    { 
  
        // Check the hash values of current window of text 
        // and pattern. If the hash values match then only 
        // check for characters on by one 
        if ( p == t ) 
        { 
            /* Check for characters one by one */
            for (j = 0; j < M; j++) 
            { 
                if (txt[i+j] != pat[j]) 
                    break; 
            } 
  
            // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1] 
            if (j == M) 
                printf("Pattern found at index %d \n", i); 
        } 
  
        // Calculate hash value for next window of text: Remove 
        // leading digit, add trailing digit 
        if ( i < N-M ) 
        { 
            t = (d*(t - txt[i]*h) + txt[i+M])%q; 
  
            // We might get negative value of t, converting it 
            // to positive 
            if (t < 0) 
            t = (t + q); 
        } 
    } 
} 

//Driver 
__host__
void TestQS() 
{ 
	char text[] = "ABAAAABAACD"; 
	char pat[] = "ABA"; 
	search(text, pat); 
	
} 
__host__
void RKTest() 
{ 
    char txt[] = "GEEKS FOR GEEKS"; 
    char pat[] = "GEEK"; 
    int q = 101; // A prime number 
    RK(pat, txt, q); 
   
} 

#endif