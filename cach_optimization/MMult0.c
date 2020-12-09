#include <stdio.h>
#include <stdlib.h>

#define A(i,j) a[ (i)*n + (j) ]
#define B(i,j) b[ (i)*n + (j) ]
#define C(i,j) c[ (i)*n + (j) ]
/* Routine for computing C = A * B + C */

int min(int a, int b)
{
	return a < b ? a : b;
}

void MY_MMult( int n, double *a, double *b, double *c)
{
	int i, j, k, bj, bk;
	double r;
	int blockSize = 36;

	for(bk = 0; bk < n; bk+=blockSize){
		for(bj = 0; bj < n; bj+=blockSize){
			for(i = 0; i < n; ++i){
				for(k = bk; k < min(bk + blockSize, n); k++){
					r = A(i, k);
					for(j = bj; j < min(bj + blockSize, n); j+=4){	
						C(i,j) += r * B(k,j);		
						C(i,j+1) += r * B(k, j+1);
						C(i,j+2) += r * B(k, j+2);
						C(i,j+3) += r * B(k, j+3);	
					}
				}
			}
		}
	}
	



}

