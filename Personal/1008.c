#include <stdio.h>

double result;
int A, B;

int main(){
	scanf("%d %d", &A, &B);
	result = (double)A/B;
	printf("%.9lf\n", result);
}
