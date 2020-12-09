#include <stdio.h>
#include <stdlib.h>

int main_MMult(char* name, void*);
void REF_MMult( int n, double *a, double *b, double *c);
void MY_MMult( int n, double *a, double *b, double *c);

int main ()
{
	/* run a simple mm */
	main_MMult ("simple", REF_MMult);

	/* run your optimized mm */
	main_MMult ("optimized", MY_MMult);

	return 0;
}
